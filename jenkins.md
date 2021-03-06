# Jenkins 简单使用

1. 安装jenkins

   下载jenkins的RPM包：http://pkg.jenkins-ci.org/redhat/jenkins-1.641-1.1.noarch.rpm

   ```bash
   # 安装jenkins
   rpm -ivh jenkins-1.641-1.1.noarch.rpm
   ```

2. 配置jenkins

   ```bash
   # 查看jenkins服务状态
   systemctl status jenkins
   # 查看jenkins是否开机启动
   systemctl is-enabled jenkins
   # 禁止jenkins开机启动
   systemctl disable jenkins
   # 启动jenkins服务
   systemctl start jenkins
   # 重启jenkins服务
   systemctl restart jenkins
   # 关闭防火墙
   systemctl status firewalld
   # 设置jenkins的contextPath（修改/etc/sysconfig/jenkins的JENKINS_ARGS）
   JENKINS_ARGS="--prefix=/jenkins"
   ```

3. 安装Git插件

   https://wiki.jenkins-ci.org/display/JENKINS/Git+Plugin

   **注意**：要先安装Git插件依赖的其他插件

   下载对应插件的hpi文件，并上传安装即可

4. 配置用户登录

   系统管理 > Configure Global Security

   - 勾选启用安全

     访问控制 > 安全域 > Jenkins专有用户数据库（不要勾选“允许用户注册”）

     授权策略 > 项目矩阵授权策略 > 输入用户名 > 添加，然后勾选所有权限（初始创建管理员账号）

     > 安全矩阵和项目矩阵授权策略的配置是一模一样的，唯一的区别是项目矩阵授权策略支持在Job的配置页面再次配置授权策略。

   - 勾选Enable Slave → Master Access Control

   - 保存，会提示创建第一个用户，输入用户名、密码等信息保存即可

   - 后续创建用户方法：系统管理 > 管理用户 > 新建用户，创建好用户后需要在Configure Global Security里为其赋权限，至少要赋予Overall/Read用户才能登录jenkins

5. 创建简单的任务

   为了可以自动从版本库Checkout代码，需要配置Jenkins的认证信息，以Git为例：

   - 为Jenkins用户生成SSH密钥，使用`ssh-keygen`命令

   - 将Jenkins用户的~/.ssh/id_rsa.pub的内容添加到git用户的~/.ssh/authorized_keys文件中

   - 在Jenkins中添加Credentials：首页 > \[左侧\]Credentials > \[右侧\]Global credentials > \[左侧\]Add 

      + Kind：SSH Username with private key
      + Scope：Global
      + Username：jenkins
      + Private Key：From the Jenkins master ~/.ssh
      + 点击OK

   - 如果是使用类似GitBucket的服务器，则可以直接将Jenkins用户的~/.ssh/id_rsa.pub的内容添加到对应GitBucket用户的SSH Keys列表里

   接下开始创建任务，在Jenkins首页点击左侧“新建”，然后输入Item名称，选择“构建一个自由风格的软件项目”，点击OK进入任务配置界面：

   - 输入“描述”

   - 勾选启用项目安全，然后添加相应的用户/组到安全矩阵，并勾选对应的权限，只读权限只需勾选Job/Read即可

   - 源码管理选择Git，输入Repository URL，如：ssh://username@git.hostname/username/HelloJenkins.git，从Credentials里选择之前创建好的认证信息

   - 在“构建触发器”里勾选“Poll SCM”，可以指定（crontab格式）时间来进行定时构建

   - 最后在“构建”部分，配置项目构建的命令，如Ant，可以指定运行特定的target

   - 最后“保存”即可

   创建好之后，可以在首页任务列表中看到新加的任务，点击任务名称，查看任务详情，可点击“立即构建”构建当前任务。

# 基于Docker Desktop for Windows搭建Jenkins

1. 安装Jenkins

   ```bash
   # 拉取jenkins镜像
   docker pull jenkinsci/blueocean
   # 给镜像打tag
   docker tag jenkinsci/blueocean:latest jenkins:2.176.3
   # 启动jenkins
   docker run --name jenkins -u root --rm -d -p 8080:8080 -p 50000:50000 -v d:/tmp/jenkins:/var/jenkins_home -v /var/run/docker.sock:/var/run/docker.sock jenkins:2.176.3
   ```

   对于内网代理环境需要配置JDK的信任证书列表，添加内部CA证书，执行`docker exec -it jenkins bash`进入容器，执行以下命令

   ```bash
   # 以下命令在容器内执行
   mkdir $JENKINS_HOME/keystore/
   cp $JAVA_HOME/jre/lib/security/cacerts $JENKINS_HOME/keystore/
   cd $JENKINS_HOME/keystore/
   chmod u+w cacerts
   # Save cacerts from browser: ca-internal.cer
   # 1. Open "Certificate" dialog
   # 2. Choose the internal certificate in "Certification Path" tab
   # 3. Press "View Certificate" button, click "Details" tab in new dialog
   # 4. Press "Copy to File..." button to export certificate
   # 5. Choose "DER encoded binary X.509(.CER)" format in wizard
   # 6. The default password of "cacert" is "changeit"
   keytool -import -alias cacerts -keystore ./cacerts -file ./ca-internal.cer
   ```

   同时为容器内用户生成ssh-key

   ```bash
   # 在容器内执行如下命令会生成~/.ssh/id_rsa和~/.ssh/id_rsa.pub两个文件
   ssh-keygen -t rsa -b 4096 -C jenkins@example.com
   ```

   > 这一步本来不是必要的，因为jenkins的ssh-key认证可以直接在页面配置。但Docker Desktop for Windows映射的外部目录和文件默认权限很大，导致构建过程中使用ssh拉取代码可能会报错：
   >
   > ```txt
   > @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   > @         WARNING: UNPROTECTED PRIVATE KEY FILE!          @
   > @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   > Permissions 0555 for '/var/jenkins_home/caches/git-6f2acf0d475addcc338566f8161868da@tmp/ssh7982434827163720833.key' are too open.
   > It is required that your private key files are NOT accessible by others.
   > This private key will be ignored.
   > ```
   >
   > 因此这里选择直接使用jenkins容器内的ssh-key做认证，页面上不再配置Credentials

   最后使用如下命令启动jenkins容器

   ```bash
   # 先停止之前启动的jenkins容器
   docker stop jenkins
   # 使用新的参数启动jenkins容器，主要是添加了代理和信任证书相关的参数
   docker run ^
     --name jenkins -u root ^
     -d -p 9999:8080 -p 50000:50000 ^
     -v d:/tmp/jenkins:/var/jenkins_home ^
     -v /var/run/docker.sock:/var/run/docker.sock ^
     -e http_proxy=proxy.host.name:port ^
     -e https_proxy=proxy.host.name:port ^
     -e JAVA_OPTS="-Djavax.net.ssl.trustStore=/var/jenkins_home/keystore/cacerts -Dhttp.proxyHost=proxy.host.name -Dhttp.proxyPort=port -Dhttp.nonProxyHosts=localhost|127.0.0.1 -Dhttps.proxyHost=proxy.host.name -Dhttps.proxyPort=port" ^
     jenkins:2.176.3
   # 查看启动过程中的日志
   docker logs -f jenkins
   ```

2. 打开 http://localhost:9999 登录jenkins，根据提示安装推荐的插件并创建管理账号。初始管理员密码可以从终端日志中查看

3. 安装gitbucket

   ```bash
   # 拉取gitbucket镜像
   docker pull gitbucket/gitbucket
   # 给镜像打tag
   docker tag gitbucket/gitbucket:latest gitbucket:4
   # 启动gitbucket
   docker run --name gitbucket -d -p 8888:8080 -p 29418:29418 -v d:/tmp/gitbucket:/gitbucket -e MAX_FILE_SIZE=10485760 gitbucket:4
   ```

   打开 http://localhost:8888 登录gitbucket，初始用户名密码：root/root，开启SSH access，并设置SSH host和SSH port

   创建一个Git Repository：`hello-jenkins`，编写并提交一个简单的基于maven的hello world程序

4. 登录jenkins容器，将~/.ssh/id_rsa.pub文件内容添加到gitbucket的root用户的SSH Keys列表中，命名为`jenkins`。在jenkins容器内尝试拉取上一步创建的示例仓库：

   ```bash
   git ls-remote -h ssh://git@hostname:29418/root/hello-jenkins.git HEAD
   ```

   首次执行以上命令会出现如下提示：

   ```txt
   The authenticity of host '[x.x.x.x]:29418 ([x.x.x.x]:29418)' can't be established.
   RSA key fingerprint is SHA256:5ZjG3IvYKcJf2VJ4TXYT1MKJ99yCMgOZ6vwLQ8Y3Osw.
   Are you sure you want to continue connecting (yes/no)?
   ```

   输入：yes，回车，提示如下信息，完成

   ```txt
   Warning: Permanently added '[x.x.x.x]:29418' (RSA) to the list of known hosts.
   ```

5. 拉取并配置maven镜像，为maven构建使用本地缓存做准备

   ```bash
   docker pull maven:3.6.1-jdk-8
   ```

   假设本地maven仓库路径为`D:\repo\maven`（默认为`%USERPROFILE%\.m2`），复制自定义的`settings.xml`到该目录，缓存的jar包应该在`D:\repo\maven\repository`目录。

   **注意**：将`settings.xml`中的`<localRepository>.....</localRepository>`内容注释掉

6. 在hello-jenkins仓库的根目录添加文件Jenkinsfile，内容如下：

   ```groovy
   pipeline {
       agent {
           docker {
               image 'maven:3.6.1-jdk-8'
               args '-v /d/repo/maven:/root/.m2'
           }
       }
       stages {
           stage('Build') {
               steps {
                   sh 'mvn -B -DskipTests clean package'
               }
           }
      stage('Test') {
               steps {
                   sh 'mvn test'
               }
               post {
                   always {
                       junit 'target/surefire-reports/*.xml'
                   }
               }
           }
   }
   ```

   以上，`args`配置就是将本地maven仓库映射到容器中，这样每次maven构建就可以使用缓存的jar包。提交Jenkinsfile，push到远程仓库

7. 以上操作执行完成后，可以将jenkins容器保存为新的镜像

   ```bash
   # 保存jenkins容器的改动到新的镜像
   docker commit jenkins jenkins:2.176.3-custom
   docker stop jenkins
   docker rm jenkins
   # 使用新的镜像启动jenkins容器
   docker run ^
     --name jenkins -u root ^
     -d -p 9999:8080 -p 50000:50000 ^
     -v d:/tmp/jenkins:/var/jenkins_home ^
     -v /var/run/docker.sock:/var/run/docker.sock ^
     -e http_proxy=proxy.host.name:port ^
     -e https_proxy=proxy.host.name:port ^
     -e JAVA_OPTS="-Djavax.net.ssl.trustStore=/var/jenkins_home/keystore/cacerts -Dhttp.proxyHost=proxy.host.name -Dhttp.proxyPort=port -Dhttp.nonProxyHosts=localhost|127.0.0.1 -Dhttps.proxyHost=proxy.host.name -Dhttps.proxyPort=port" ^
     jenkins:2.176.3-custom
   ```

8. 最后参考Jenkins官方教程（[使用Maven构建Java应用程序](https://jenkins.io/zh/doc/tutorials/build-a-java-app-with-maven/ "使用Maven构建Java应用程序")）测试hello-jenkins示例

# 基于Docker(on Linux)搭建Jenkins

1. 如果是在Linux下，可以直接使用如下docker-compose文件来启动jenkins和相关服务，**无需定制Docker镜像**

   ```yaml
   # docker-compose.yml
   version: '3'
   services:
     registry:
       container_name: registry
       restart: always
       image: registry:2
       environment:
         REGISTRY_HTTP_ADDR: 0.0.0.0:5000
         REGISTRY_STORAGE_DELETE_ENABLED: "true"
       ports:
         - 80:5000
       volumes:
         - /opt/registry:/var/lib/registry

     gitbucket:
       container_name: gitbucket
       restart: always
       image: gitbucket:4
       environment:
         MAX_FILE_SIZE: 10485760
         COMPOSE_PROJECT_NAME: gitbucket
       ports:
         - 8888:8080
         - 29418:29418
       volumes:
         - /opt/gitbucket:/gitbucket

     jenkins:
       container_name: jenkins
       restart: always
       image: jenkins:2.176.3
       privileged: true
       user: root
       environment:
         http_proxy: proxy.host.name:port
         https_proxy: proxy.host.name:port
         JAVA_OPTS: -Djavax.net.ssl.trustStore=/var/jenkins_home/keystore/cacerts -Dhttp.proxyHost=proxy.host.name -Dhttp.proxyPort=port -Dhttp.nonProxyHosts=localhost|127.0.0.1 -Dhttps.proxyHost=proxy.host.name -Dhttps.proxyPort=port
         COMPOSE_PROJECT_NAME: jenkins
       ports:
         - 9999:8080
         - 50000:50000
       volumes:
         - /opt/jenkins:/var/jenkins_home
         - /var/run/docker.sock:/var/run/docker.sock
   ```

   使用如下命令启动容器

   ```bash
   docker-compose up -d
   ```

2. 下面参考jenkins官方教程（[使用Maven构建Java应用程序](https://jenkins.io/zh/doc/tutorials/build-a-java-app-with-maven/ "使用Maven构建Java应用程序")）构造pipeline示例

   **注意**: 在开始下面的操作之前请先为Jenkins安装以下几个插件
   * 用于获取maven的pom.xml文件中的信息: `Pipeline Utility Steps`
   * 用于自动运行tag构建: `Basic Branch Build Strategies`
   * 用于在Readme中添加Jenkins Build Status Icon: `Embeddable Build Status`
   * 用于发送HTTP请求: `Http Request`

3. 在GitBucket上创建Git仓库示例: `hello-jenkins`

   使用如下命令生成代码:

   ```bash
   mvn archetype:generate \
     -DarchetypeArtifactId=jersey-quickstart-grizzly2 \
     -DarchetypeGroupId=org.glassfish.jersey.archetypes \
     -DinteractiveMode=false \
     -DgroupId=com.my.project \
     -DartifactId=hello-jenkins \
     -Dpackage=com.my.project \
     -DarchetypeVersion=2.29.1
   ```

   修改`pom.xml`

   ```xml
   <project xmlns="http://maven.apache.org/POM/4.0.0"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">

       <modelVersion>4.0.0</modelVersion>

       <groupId>com.my.project</groupId>
       <artifactId>hello-jenkins</artifactId>
       <packaging>jar</packaging>
       <version>1.0-SNAPSHOT</version>
       <name>hello-jenkins</name>

       <dependencyManagement>
           <dependencies>
               <dependency>
                   <groupId>org.glassfish.jersey</groupId>
                   <artifactId>jersey-bom</artifactId>
                   <version>${jersey.version}</version>
                   <type>pom</type>
                   <scope>import</scope>
               </dependency>
           </dependencies>
       </dependencyManagement>

       <dependencies>
           <dependency>
               <groupId>org.glassfish.jersey.containers</groupId>
               <artifactId>jersey-container-grizzly2-http</artifactId>
           </dependency>
           <dependency>
               <groupId>org.glassfish.jersey.inject</groupId>
               <artifactId>jersey-hk2</artifactId>
           </dependency>

           <!-- uncomment this to get JSON support:
            <dependency>
               <groupId>org.glassfish.jersey.media</groupId>
               <artifactId>jersey-media-json-binding</artifactId>
           </dependency>
           -->
           <dependency>
               <groupId>junit</groupId>
               <artifactId>junit</artifactId>
               <version>4.12</version>
               <scope>test</scope>
           </dependency>
       </dependencies>

       <build>
           <plugins>
               <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-compiler-plugin</artifactId>
                   <version>2.5.1</version>
                   <inherited>true</inherited>
                   <configuration>
                       <source>1.8</source>
                       <target>1.8</target>
                   </configuration>
               </plugin>
               <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-jar-plugin</artifactId>
                   <version>2.4</version>
                   <configuration>
                       <archive>
                           <addMavenDescriptor>false</addMavenDescriptor>
                           <manifest>
                               <addClasspath>true</addClasspath>
                               <classpathPrefix>lib</classpathPrefix>
                               <mainClass>com.my.project.Main</mainClass>
                           </manifest>
                           <manifestEntries>
                               <Class-Path>.</Class-Path>
                           </manifestEntries>
                       </archive>
                   </configuration>
               </plugin>
               <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-dependency-plugin</artifactId>
                   <version>3.1.1</version>
                   <executions>
                       <execution>
                           <id>copy-dependencies</id>
                           <phase>package</phase>
                           <goals>
                               <goal>copy-dependencies</goal>
                           </goals>
                           <configuration>
                               <includeScope>runtime</includeScope>
                               <outputDirectory>${project.build.directory}/lib</outputDirectory>
                           </configuration>
                       </execution>
                   </executions>
               </plugin>
           </plugins>
       </build>

       <properties>
           <jersey.version>2.29.1</jersey.version>
           <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
       </properties>
   </project>
   ```

   创建`Dockerfile`

   ```dockerfile
   FROM openjdk:8u222-jdk
   LABEL Author="Aquaman"
   VOLUME [ "/tmp" ]
   WORKDIR /opt/app/
   COPY target/lib lib
   COPY target/*.jar ./app.jar
   ENTRYPOINT ["java", "-jar", "app.jar"]
   ```

   创建`Jenkinsfile`

   ```groovy
   pipeline {

       // 所有stage都将在这个maven容器中执行
       agent {
           docker {
               image 'maven:3.6.2-jdk-8'
               // 挂载maven本地仓库、docker命令、docker套接字
               // 挂载docker是为了在maven容器内构建docker镜像或启停docker容器
               args '-v /root/.m2:/root/.m2 -v /var/run/docker.sock:/var/run/docker.sock -v /usr/bin/docker:/usr/bin/docker'
           }
       }

       // 全局环境变量
       environment {
           // docker image name，使用pom.xml中的artifactId
           IMAGE_NAME = "docker.local/${readMavenPom().getArtifactId()}"
           // docker image tag，使用pom.xml中的version
           VERSION = readMavenPom().getVersion()
           // docker container name，使用pom.xml中的artifactId
           CONTAINER_NAME = readMavenPom().getArtifactId()
           // docker container expose port
           PORT = 8081
       }

       stages {
           stage('Build') {
               steps {
                   sh 'mvn -B -DskipTests clean package'
               }
           }
           stage('Test') {
               steps {
                   sh 'mvn test'
               }
               post {
                   always {
                       junit 'target/surefire-reports/*.xml'
                   }
               }
           }
           stage('Docker Image') {
               steps {
                   // 首先停止并删除正在运行的容器、镜像，然后build镜像并push
                   sh '''
                   if [ "$(docker ps -a | grep ${CONTAINER_NAME})" ]; then
                       docker stop ${CONTAINER_NAME}
                   fi
                   if [ "$(docker ps -aq -f status=exited -f name=${CONTAINER_NAME})" ]; then
                       docker rm ${CONTAINER_NAME}
                   fi
                   if [ ! -z $(docker images -q ${IMAGE_NAME}:${VERSION}) ]; then
                       docker rmi ${IMAGE_NAME}:${VERSION}
                   fi
                   echo 'Build Docker Image'
                   docker build -t ${IMAGE_NAME}:${VERSION} .
                   echo 'Push Docker Image'
                   docker push ${IMAGE_NAME}:${VERSION}
                   '''
               }
           }
           stage('Deploy') {
               steps {
                   // 直接启动容器
                   sh '''
                   echo 'Start Application with Docker'
                   docker run -d -p ${PORT}:8080 --name ${CONTAINER_NAME} ${IMAGE_NAME}:${VERSION}
                   '''
               }
           }
       }
   }
   ```

   为了方便测试，需要修改`Main.java`

   - 去掉`main`方法中的如下内容:

     ```java
     System.in.read();
     server.stop();
     ```

   - 修改变量`BASE_URI`的值为：`http://0.0.0.0:8080/myapp/`

   基于以上示例代码构建的pipeline执行成功后，如下请求应该可以正常返回：`Got it!`

   ```txt
   GET http://hostname:8081/myapp/myresource
   ```

4. 如下，是一个多分支构建的`Jenkinsfile`的示例。在创建多分支pipeline时，注意配置：

   * `Branch Sources`-->`Behaviours`，添加`Discover tags`以启用tag构建
   * `Branch Sources`-->`Build strategies`，添加`Tags`，使用默认配置
   * `Branch Sources`-->`Property strategy`，添加Property `Suppress automatic SCM triggering`，可以防止扫描分支和Tag的时候自动触发构建
   * `Scan Multibranch Pipeline Triggers`，勾选`Periodically if not otherwise run`，选择时间间隔`Interval`为`5 minutes`

   ```groovy
   pipeline {

       agent {
           docker {
               image 'maven:3.6.2-jdk-8'
               args '-v /root/.m2:/root/.m2 -v /var/run/docker.sock:/var/run/docker.sock -v /usr/bin/docker:/usr/bin/docker'
           }
       }

       environment {
           IMAGE_NAME = "docker.local/${readMavenPom().getArtifactId()}"
           IMAGE_TAG = getImageTag()
           CONTAINER_NAME = readMavenPom().getArtifactId()
           PORT = 8080
           JMX_PORT = 18080
           JAVA_OPTS="-server -Xms128m -Xmx128m -Djava.rmi.server.hostname=10.0.76.10 -Dcom.sun.management.jmxremote=true -Dcom.sun.management.jmxremote.port=${JMX_PORT} -Dcom.sun.management.jmxremote.rmi.port=${JMX_PORT} -Dcom.sun.management.jmxremote.ssl=false -Dcom.sun.management.jmxremote.authenticate=false"
       }

       stages {

           stage('Debug') {
               steps {
                   sh 'printenv'
               }
           }

           stage('Build') {
               steps {
                   sh 'mvn -B -DskipTests clean package'
               }
           }

           stage('Test') {
               steps {
                   sh 'mvn test'
               }
               post {
                   always {
                       junit 'target/surefire-reports/*.xml'
                   }
               }
           }

           stage('Docker Image') {
               steps {
                   sh '''
                   # save old image id
                   oldImageId=`docker images -q ${IMAGE_NAME}:${IMAGE_TAG}`

                   # build new image
                   echo 'Build Docker Image'
                   docker build -t ${IMAGE_NAME}:${IMAGE_TAG} .
                   echo 'Push Docker Image'
                   docker push ${IMAGE_NAME}:${IMAGE_TAG}

                   # remove old image
                   if [ ! -z ${oldImageId} ]; then
                       if [ ! -z $(docker ps -aq -f ancestor=${oldImageId}) ]; then
                           docker rm -f `docker ps -aq -f ancestor=${oldImageId}`
                       fi
                       docker rmi -f ${oldImageId}
                   fi
                   '''
               }
           }

           stage('Deliver Development') {
               when {
                   allOf {
                       not { branch 'master' };
                       not { buildingTag() }
                   }
               }
               steps {
                   sh '''
                   echo 'Deliver Development'
                   if [ "$(docker ps -a | grep ${CONTAINER_NAME})" ]; then
                       docker rm -f ${CONTAINER_NAME}
                   fi
                   docker run -d -p ${PORT}:8080 -p ${JMX_PORT}:${JMX_PORT} --name ${CONTAINER_NAME} -e JAVA_OPTS="${JAVA_OPTS}" ${IMAGE_NAME}:${IMAGE_TAG}
                   '''
               }
           }

           stage('Deploy Production') {
               when {
                   buildingTag()
               }
               steps {
                   input message: 'Deploy to Production? (Click "Proceed" to continue)'
                   sh '''
                   echo 'Depoly Production'
                   if [ "$(docker ps -a | grep ${CONTAINER_NAME})" ]; then
                       docker rm -f ${CONTAINER_NAME}
                   fi
                   docker run -d -p ${PORT}:8080 -p ${JMX_PORT}:${JMX_PORT} --name ${CONTAINER_NAME} -e JAVA_OPTS="${JAVA_OPTS}" ${IMAGE_NAME}:${IMAGE_TAG}
                   '''
               }
           }

       }

   }

   /* Get Docker Image Tag */
   def getImageTag() {
       if(env.TAG_NAME) {
           return env.TAG_NAME
       } else if(env.BRANCH_NAME) {
           if(env.BRANCH_NAME == "master") {
               return "latest"
           } else {
               return env.BRANCH_NAME
           }
       } else {
           return env.GIT_COMMIT
       }
   }
   ```

   ```bash
   # 提交tag触发构建
   git tag 1.0
   git push origin --tags
   ```

5. 如果需要在pipeline中同时构建前台代码，可以在上例中的Build阶段之前加入如下stage，实现前台代码编译，并在后续构建docker镜像时将编译好的文件复制到镜像中

   ```groovy
   // reuseNode参数指定该agent与pipeline顶层agent使用相同的workspace
   // https://jenkins.io/doc/book/pipeline/syntax/#agent
   stage('Build Portal') {
       agent {
           docker {
               image 'node:12.13.0-alpine'
               args '-u root -p 3000:3000'
               reuseNode true
           }
       }
       steps {
           sh 'cd ./portal/ && npm install && npm run build'
       }
   }
   ```

6. 对于maven本地仓库的共享，可能会需要将Windows机器的目录共享给Linux，可通过如下方式实现

   首先在Windows上开启maven本地仓库目录的共享：`在目录上右键`-->`属性`-->`共享`-->`共享(S)...`

   在Linux机器上挂载共享目录到`/root/.m2`

   ```bash
   # 安装依赖工具
   yum install cifs-utils
   # 创建.m2目录
   mkdir /root/.m2
   # 挂载Windows共享目录，执行以下命令会提示用户输入Windows系统用户的密码
   mount.cifs //windows-host-name/share-folder /root/.m2 -o user=windows-user-name
   # 挂载成功，查看状态
   df -h
   ls /root/.m2
   ```

   配置自动挂载。首先创建文件`/root/.cifs`，文件内容如下

   ```txt
   username=windows-user-name
   password=windows-user-password
   ```

   然后修改`/etc/fstab`，添加如下内容

   ```txt
   //windows-host-name/share-folder /root/.m2 cifs credentials=/root/.cifs 0 0
   ```

   最后，重启Linux机器，查看挂载状态

   ```bash
   df -h
   ls /root/.m2
   ```
