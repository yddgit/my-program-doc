# 介绍

Google Cloud project是用来组织和管理资源的实体，project中通常包含各种资源和服务，如VM、DB、网络、权限、安全规则等。project可以是按团队的，也是可以是按产品的。

# 角色

|Name|Permission|
|-|-|
|viewer|执行不影响状态的只读操作，如查看现有资源和数据|
|editor|拥有viewer的所有权限，还可以执行会修改状态的操作，如更改现有资源|
|owner|拥有editor的所有权限，还可以：管理项目的角色、权限和所有资源；为项目设置结算信息|

# API

可以通过代码或类库直接调用API，实现对Google Cloud上资源和服务的操作，包括计算、网络、存储、机器学习等。

# 基础

> VM这种Compute Engine资源一般位于特定的地理位置，所以创建时需要选择其所在的区域，如：
> |Regions|Zones|
> |-|-|
> |Western US|us-west1-a, us-west1-b|
> |Central US|us-central1-a, us-central1-b, us-central1-d, us-central1-f|
> |Eastern US|us-east1-b, us-east1-c, us-east1-d|
> |Western Europe|europe-west1-b, europe-west1-c, europe-west1-d|
> |Eastern Asia|asia-east1-a, asia-east1-b, asia-east1-c|

## 创建VM(Linux)

两种方式：

1. 通过Cloud Console(Web界面)创建：Compute Engine → VM Instances → CREATE INSTANCE → Name/Region/Zone/Series/Machine Type/Boot Disk/Firewall(allow http traffic) → Create → SSH
2. 通过gcloud创建
   ```sh
   # 创建虚拟机
   gcloud compute instances create gcelab2 --machine-type e2-medium --zone us-central1-f
   # 查看创建虚拟机的所有默认参数
   gcloud compute instances create --help
   # ssh连接(注意第一次连接需要生成一个passphrase为空的ssh key)
   gcloud compute ssh gcelab2 --zone us-central1-f
   ```
## 创建VM(Windows)

- Compute Engine → VM Instances → CREATE INSTANCE → Name/Region/Zone/Series/Boot Disk(Change operation system to Windows Server) → Create
- 打开Cloud Shell，配置RDP
  ```sh
  # 检测windows启动状态，如果提示Y/n，需要所根据实际情况选择，比如：提示的zone与instance的zone不一致时就要选n
  gcloud compute instances get-serial-port-output [instance]
  # 设置登录账号和密码
  gcloud compute reset-windows-password [instance] --zone us-east1-b --user [username]
  # 登录成功后可以验证一下本机和远程主机之间的Copy/Paste功能
  ```

## Cloud Shell和gcloud

Cloud Shell是一种装有开发者工具的虚拟机，提供5GB主目录，运行在Google Cloud上，可以提供命令行访问GCP资源的能力。在 GCP Console 右上角的工具栏中，点击“打开 Cloud Shell”按钮，点击“继续”，稍候连接成功后，会设置环境变量PROJECT_ID。gcloud是Cloud Shell中预装的命令行工具，支持Tab补全

```sh
# 列出有效账号名称
gcloud auth list
# 列出项目ID
gcloud config list project
# 设置region
gcloud config set compute/region us-central1
# 查看region设置
gcloud config get-value compute/region
# 设置zone
gcloud config set compute/zone us-central1-b
# 查看zone设置
gcloud config get-value compute/zone
# 查看project ID
gcloud config get-value project
# 查看project detail
gcloud compute project-info describe --project $(gcloud config get-value project)

# 设置PROJECT_ID
export PROJECT_ID=$(gcloud config get-value project)
# 设置ZONE
export ZONE=$(gcloud config get-value compute/zone)
# 确认设置的环境变量
echo -e "PROJECT ID: $PROJECT_ID\nZONE: $ZONE"

# 创建虚拟机
#  gcloud compute - 管理Compute Engine资源
#  instance create - 创建新实例
#  gcelab2 - 虚拟机名称
#  --machine-type - 指定机器类型为n1-standard-2
#  --zone 指定VM创建位置
gcloud compute instances create gcelab2 --machine-type n1-standard-2 --zone $ZONE

# 获取命令帮助
gcloud compute instances create --help
# 获取简洁的使用指南 -h
gcloud -h
# 获取详细的使用指南 --help or gcloud help
gcloud config --help
gcloud help config

# 查看各配置列表
gcloud config list
# 查看所有配置
gcloud config list --all
# 列出组件
gcloud components list

# 列出可用计算实例
gcloud compute instances list
# 列出指定name的实例
gcloud compute instances list --filter="name=('gcelab2')"
# 列出防火墙规则
gcloud compute firewall-rules list
# 列出默认防火墙规则
gcloud compute firewall-rules list --filter="network='default'"
# 列出允许ICMP的默认防火墙规则
gcloud compute firewall-rules list --filter="NETWORK:'default' AND ALLOW:'icmp'"

# 连接到VM
gcloud compute ssh gcelab2 --zone $ZONE
# 安装nginx
sudo apt install -y nginx
# 退出VM
exit

# 列出防火墙规则
gcloud compute firewall-rules list
# 为虚拟机添加tag
gcloud compute instances add-tags gcelab2 --tags http-server,https-server
# 更新防火墙允许规则
gcloud compute firewall-rules create default-allow-http --direction=INGRESS --priority=1000 --network=default --action=ALLOW --rules=tcp:80 --source-ranges=0.0.0.0/0 --target-tags=http-server
# 列出防火墙规则
gcloud compute firewall-rules list --filter=ALLOW:'80'
# 验证http通信
curl http://$(gcloud compute instances list --filter=name:gcelab2 --format='value(EXTERNAL_IP)')

# 查看系统日志
gcloud logging logs list
# 查看与Compute Engine资源有关的日志
gcloud logging logs list --filter="compute"
# 查看与gce_instance资源类型相关的日志
gcloud logging read "resource.type=gce_instance" --limit 5
# 读取特定虚拟机的日志
gcloud logging read "resource.type=gce_instance AND labels.instance_name='gcelab2'" --limit 5
```

## Kubernetes Engine

Google Kubernetes Engine (GKE) 提供了一个受管环境，您可以使用 Google 基础架构在其中部署、管理和调节容器化应用。

Google Cloud上的Kubernetes
- 针对 Compute Engine 实例提供的负载平衡功能
- 节点池（可用于在集群中指定节点子集以提高灵活性）
- 自动扩缩集群的节点实例数量
- 自动升级集群的节点软件
- 节点自动修复，以保持节点的正常运行和可用性
- 利用 Cloud Monitoring 进行日志记录和监控，让您可以清楚了解自己集群的状况

```sh
# 设置默认region
gcloud config set compute/region us-east1
# 设置默认zone
gcloud config set compute/zone us-east1-b
# 创建GKE集群
gcloud container clusters create --machine-type=e2-medium lab-cluster
# 获取集群身份验证凭据
gcloud container clusters get-credentials lab-cluster
# 部署应用
kubectl create deployment hello-server --image=gcr.io/google-samples/hello-app:1.0
# 创建Service
kubectl expose deployment hello-server --type=LoadBalancer --port 8080
# 检查Service状态
kubectl get service
# 打开浏览器访问：http://[EXTERNAL-IP]:8080
# 删除集群
gcloud container clusters delete lab-cluster
```

## 网络和HTTP Load Balancer

网络负载平衡器

```sh
# 设置默认region和zone
gcloud config set compute/region us-east5
gcloud config set compute/zone us-east5-c

# 创建多个网络服务器实例，并指定相同的tags，后续可以一次性同时引用这些实例
# 如下为每个实例安装了Apache，并设置了唯一首页
gcloud compute instances create www1 \
  --zone=us-east5-c \
  --tags=network-lb-tag \
  --machine-type=e2-medium \
  --image-family=debian-11 \
  --image-project=debian-cloud \
  --metadata=startup-script='#!/bin/bash
    apt-get update
    apt-get install apache2 -y
    service apache2 restart
    echo "<h3>Web Server: www1</h3>" | tee /var/www/html/index.html'
gcloud compute instances create www2 \
  --zone=us-east5-c \
  --tags=network-lb-tag \
  --machine-type=e2-medium \
  --image-family=debian-11 \
  --image-project=debian-cloud \
  --metadata=startup-script='#!/bin/bash
    apt-get update
    apt-get install apache2 -y
    service apache2 restart
    echo "<h3>Web Server: www2</h3>" | tee /var/www/html/index.html'
gcloud compute instances create www3 \
  --zone=us-east5-c \
  --tags=network-lb-tag \
  --machine-type=e2-medium \
  --image-family=debian-11 \
  --image-project=debian-cloud \
  --metadata=startup-script='#!/bin/bash
    apt-get update
    apt-get install apache2 -y
    service apache2 restart
    echo "<h3>Web Server: www3</h3>" | tee /var/www/html/index.html'
# 创建允许外部流量访问的防火墙规则
gcloud compute firewall-rules create www-firewall-network-lb \
  --target-tags network-lb-tag --allow tcp:80
# 查看所有实例
gcloud compute instances list
# 验证每个VM是否在运行
curl http://[IP_ADDRESS]

# 创建Load Balancer，分配静态外部IP地址
gcloud compute addresses create network-lb-ip-1 \
  --region us-east5
# 添加http运行状况检查
gcloud compute http-health-checks create basic-check
# 向以上VM实例所在region添加目标池，并执行必要的运行状况检查
gcloud compute target-pools create www-pool \
  --region us-east5 --http-health-check basic-check
# 将实例添加到目标池
gcloud compute target-pools add-instances www-pool \
  --instances www1,www2,www3
# 添加转发规则
gcloud compute forwarding-rules create www-rule \
  --region us-east5 \
  --ports 80 \
  --address network-lb-ip-1 \
  --target-pool www-pool
# 查看Load Balancer使用的www-rule转发规则的外部IP地址
gcloud compute forwarding-rules describe www-rule --region us-east5
# 使用curl命令访问外部IP地址，Ctrl+C停止该命令
IPADDRESS=$(gcloud compute forwarding-rules describe www-rule --region us-east5 --format="json" | jq -r .IPAddress)
echo $IPADDRESS
while true; do curl -m1 $IPADDRESS; done
```

HTTP(S) 负载平衡器

以Google Front End(GFE)为基础实现。这些GFE分布于全球各地，可通过Google的全球网络和控制层面协同动作。可以将网址规则配置为将一些网址路由到一组实例，然后将其他网址路由到其他实例。请求始终会被路由到有足够处理能力的距离用户最近的那组实例。

如果使用Compute Engine做为后端，那所有VM必须属于某个实例组，这个实例组提供一组VM，用于运行外部HTTP Load Balancer的后端服务器。

```sh
# 创建Load Balancer模板
gcloud compute instance-templates create lb-backend-template \
  --region=us-east5 \
  --network=default \
  --subnet=default \
  --tags=allow-health-check \
  --machine-type=e2-medium \
  --image-family=debian-11 \
  --image-project=debian-cloud \
  --metadata=startup-script='#!/bin/bash
    apt-get update
    apt-get install apache2 -y
    a2ensite default-ssl
    a2enmod ssl
    vm_hostname="$(curl -H "Metadata-Flavor:Google" \
    http://169.254.169.254/computeMetadata/v1/instance/name)"
    echo "Page served from: $vm_hostname" | \
    tee /var/www/html/index.html
    systemctl restart apache2'
# 基于该模板创建一个Managed instance groups (MIGs)
# MIG可以用于操作多个特定VM上的app，实现扩容、HA、基于region(多zone)部署、自动更新
gcloud compute instance-groups managed create lb-backend-group \
  --template=lb-backend-template --size=2 --zone=us-east5-c
# 创建创建fw-allow-health-check防火墙规则。这是一种入站流量规则，允许来自Google Cloud运行状况检查系统（130.211.0.0/22 和 35.191.0.0/16）的流量
# 如下使用tags=allow-health-check来标识虚拟机
gcloud compute firewall-rules create fw-allow-health-check \
  --network=default \
  --action=allow \
  --direction=ingress \
  --source-ranges=130.211.0.0/22,35.191.0.0/16 \
  --target-tags=allow-health-check \
  --rules=tcp:80
# 实例启动后，设置一个全局静态外部IP
gcloud compute addresses create lb-ipv4-1 \
  --ip-version=IPV4 \
  --global
# 记录一下预留的IPv4地址
gcloud compute addresses describe lb-ipv4-1 \
  --format="get(address)" \
  --global
# 为Load Balancer创建运行状况检查
gcloud compute health-checks create http http-basic-check \
  --port 80
# 创建后端服务
gcloud compute backend-services create web-backend-service \
  --protocol=HTTP \
  --port-name=http \
  --health-checks=http-basic-check \
  --global
# 将实例组作为后端添加到后端服务
gcloud compute backend-services add-backend web-backend-service \
  --instance-group=lb-backend-group \
  --instance-group-zone=us-east5-c \
  --global
# 创建URL map，以将传入的请求路由到默认的后端服务
gcloud compute url-maps create web-map-http \
  --default-service web-backend-service
# 创建一个目标http代理，以将请求路由到以上URL map
gcloud compute target-http-proxies create http-lb-proxy \
  --url-map web-map-http
# 创建一条全局转发规则，以将传入的请求路由到相应的代理
gcloud compute forwarding-rules create http-content-rule \
  --address=lb-ipv4-1\
  --global \
  --target-http-proxy=http-lb-proxy \
  --ports=80
# 在Cloud Console：Network Services --> Load Balancing --> web-map-http
# 在Backend部分点击名称，确认VM运行状况良好
# 打开浏览器访问http://IP_ADDRESS，IP_ADDRESS为Load Balancer的IP地址。如果无法连接，请等待一分钟
# 浏览器会呈现一个页面，显示：Page served from: lb-backend-group-xxxx
```

## 管理Cloud资源

```sh
# 创建VM
gcloud compute instances create nucleus-jumphost-678 --machine-type f1-micro

# Kubernetes Engine
gcloud container clusters create --machine-type=n1-standard-1 jooli-cluster --zone us-east1-b
gcloud container clusters get-credentials jooli-cluster --zone us-east1-b
kubectl create deployment hello-server --image=gcr.io/google-samples/hello-app:2.0
kubectl expose deployment hello-server --type=LoadBalancer --port 8080
kubectl get service
gcloud container clusters delete jooli-cluster --zone us-east1-b

# HTTP Load Balancer
cat << EOF > startup.sh
#! /bin/bash
apt-get update
apt-get install -y nginx
service nginx start
sed -i -- 's/nginx/Google Cloud Platform - '"\$HOSTNAME"'/' /var/www/html/index.nginx-debian.html
EOF

gcloud compute instance-templates create web-server-template \
  --network=default \
  --subnet=default \
  --tags=allow-health-check \
  --machine-type=f1-micro \
  --metadata-from-file startup-script=startup.sh

gcloud compute instance-groups managed create web-server-group \
  --base-instance-name web-server \
  --template=web-server-template --size=2

gcloud compute instance-groups managed \
  set-named-ports web-server-group \
  --named-ports http:80 \

gcloud compute firewall-rules create grant-tcp-rule-213 \
  --network=default \
  --action=allow \
  --direction=ingress \
  --source-ranges=130.211.0.0/22,35.191.0.0/16 \
  --target-tags=allow-health-check \
  --rules=tcp:80

gcloud compute addresses create lb-ipv4-1 \
  --ip-version=IPV4 \
  --global

gcloud compute addresses describe lb-ipv4-1 \
  --format="get(address)" \
  --global

gcloud compute health-checks create http http-basic-check \
  --port 80

gcloud compute backend-services create web-backend-service \
  --protocol=HTTP \
  --port-name=http \
  --health-checks=http-basic-check \
  --global

gcloud compute backend-services add-backend web-backend-service \
  --instance-group=web-server-group \
  --instance-group-zone=us-east1-b \
  --global

gcloud compute url-maps create web-map-http \
  --default-service web-backend-service

gcloud compute target-http-proxies create http-lb-proxy \
  --url-map web-map-http

gcloud compute forwarding-rules create http-content-rule \
  --address=lb-ipv4-1\
  --global \
  --target-http-proxy=http-lb-proxy \
  --ports=80
```

# Google Cloud Platform Overview

## Introduce

- Cloud Computing
- IaaS/Paas/SaaS
- Network/Region/Zone
- Environment impact
- Security: Intrusion detection/Reducing insider risk/Employee Universal Second Factor(U2F) use/Software development practices
- Opensource ecosystems: TensorFlow/Kubernetes/Operations Suite
- Pricing and billing: Kubernetes/Dataproc/App Engine/Compute Engine/Budgets-Alerts-Reports-Quotas

## Resources

- Resources
- Project
- Folder
- Org

## IAM

- Basic Role: Owner/Editor/Viewer/Billing Admin
- Predefined Role: Instance Admin Role(Compute Engine)
- Custom Role: Can't apply to folder level

## Service Account

Give permission to a vm rather than a person

## Cloud Identity

Integrated with AD or LDAP

## Interacting with Google Cloud

- Google Cloud Console
- gcloud: Command line interface for Google Cloud products and services
- gsutil: Command line access to Cloud Storage
- bq: Command line tool for BigQuery
- Google Cloud Shell: 5GB home directory/debian based/utilities up to date and fully authenticated
- API: client/libraries/Java,Python,PHP,C#,Go,Node.js
- Cloud Console Mobile App

## Virtual Private Cloud

- Segment networks/Firewall rule to restrict/Static routes to forward traffic
- Network is global and can have subnets in any regions
- VPC belongs to projects, VPC talk to each other: VPC Peering / Shared VPC

## Compute Engine

- VM: Linux or Windows
- Cloud Marketplace
- Bills by seconds
- Preemptible VMs: Compute Engine has permission to terminate a job if its resources are needed elsewhere. It can only run for up to 24 hours at a time.
- Spot VMs
- Scaling VMs: scale out not up.

## Cloud Load Balancing

- Distributed/Software define/managed service
- Can put in front of: HTTP(s)/TCP/SSL/UDP
- Cross region, automatic multi-region failover

## Cloud DNS and Cloud CDN

- Cloud DNS is programmable
- Cloud CDN can be enabled with a checkbox after HTTP load balancing is set up

## Storage

- Cloud Storage: Object storage. Video/Image/Audio, Unique keys in form of URL. Versioned, Life cycle management
- Cloud SQL: MySQL/PostgreSQL/SQL Server. Can scale up to 64c+400GB+30TB
- Cloud Spanner: SQL releational database/HA/consistency/DB size exceed 2TB/High numbers of operations per second
- Firestore: NoSQL, document/collection/filtering/sorting/indexed. Can write/read/query offline, upload if device online. Free quota: 10GB free egress per month, 50000 document reads per day, 20000 document writes per day, 20000 document deletes per day, 1GB of stored data
- Cloud Bigtable: NoSQL, bigdata database service

## Container

- Compute Engine(IaaS)
- App Engine(PaaS)
- GKE: Google Kubernetes Engine
  - Load balancing for compute engine instances
  - Node pools
  - Auto scale node instance count
  - Auto upgrade node software
  - Auto repair maintain node health and availability
  - Logging and monitoring with Google operations suite
- Hybird and multi-cloud: Anthos

## Application develop

- App Engine: Standard(Container)/Flexible(Custom by Dockerfile/Image)
- Cloud Endpoints
- Apigee Edge
- Cloud Run: Only pay for handling web requests. Cloud Run does this using Buildpacks - an open source project.

## Deployment in cloud

- Cloud Functions
- Infrastructure as Code: Terraform

# Foundational Infrastructure Tasks

## Cloud Storage - Cloud Console / CLI / SDK

### Cloud Console

- Navigation menu > Cloud Storage > Browser
- Create Bucket
- Name: Use Project ID, it always be unique.
- Continue
- Region: us-east1
- Default storage class: Standard
- Access control: Uniform
- Create
- Upload file: Click bucket name created > Object tab > Upload
- Allow public access: Permssions > Principals > Add > Add principals > New principals: allUsers > Role: Cloud Storage - Storage Object Viewer > Save > Allow public access
- Verify public access: https://storage.googleapis.com/`YOUR_BUCKET_NAME`/kitten.png
- Create folder: Objects tab > Create folder > Name: folder1 > Create
- Click folder1 > Create folder > Name: folder2 > Create > Click folder2 > Upload files
- Bucket details > Click bucket name > Select folder1 > Delete > Confirm

### CLI / SDK

```sh
# active cloud shell
gcloud auth list
# list project id
gcloud config list project

# create a bucket on cloud console as before steps
# Choose: multi-region > us
# Uncheck: Enforce public access prevention on this bucket under Prevent public access
# Select: Fine-grained under Access Control

# download image: ada.jpg
curl https://upload.wikimedia.org/wikipedia/commons/thumb/a/a4/Ada_Lovelace_portrait.jpg/800px-Ada_Lovelace_portrait.jpg --output ada.jpg
# upload image to bucket
gsutil cp ada.jpg gs://YOUR-BUCKET-NAME
# remove downloaded image
rm ada.jpg

# download image from bucket
gsutil cp -r gs://YOUR-BUCKET-NAME/ada.jpg .
# copy image to a folder
gsutil cp gs://YOUR-BUCKET-NAME/ada.jpg gs://YOUR-BUCKET-NAME/image-folder/

# list content
gsutil ls gs://YOUR-BUCKET-NAME
# list detail
gsutil ls -l gs://YOUR-BUCKET-NAME/ada.jpg

# make object publicly accessible
gsutil acl ch -u AllUsers:R gs://YOUR-BUCKET-NAME/ada.jpg
# validate on cloud console: Copy URL in Public link box
# remove public access
gsutil acl ch -d AllUsers gs://YOUR-BUCKET-NAME/ada.jpg

# delete object
gsutil rm gs://YOUR-BUCKET-NAME/ada.jpg
```

## Cloud IAM

Navigation menu > IAM & Admin > IAM

|Role Name|Permissions|
|-|-|
|viewer|Permissions for read-only actions that do not affect state, such as viewing (but not modifying) existing resources or data.|
|editor|All viewer permissions, plus permissions for actions that modify state, such as changing existing resources.|
|owner|All editor permissions and permissions for the following actions: ① Manage roles and permissions for a project and all resources within the project. ② Set up billing for a project.|
|browser|Read access to browse the hierarchy for a project, including the folder, organization, and Cloud IAM policy. This role doesn't include permission to view resources in the project.|

## Cloud Monitoring

- Compute Engine > VM instances > Create instance
  - Name: lamp-1-vm
  - Region: us-central1
  - Zone: us-central1-a
  - Series: N1
  - Machine type: n1-standard-2
  - Book disk: Debian GNU/Linux 10(buster)
  - Firewall: Allow HTTP traffic
- SSH to VM

  ```sh
  sudo apt-get update
  sudo apt-get install apache2 php7.0
  sudo service apache2 start
  ```

- Verify: http://EXTERNAL-IP/
- Navigation menu > Monitoring
- Install Ops agents

  ```sh
  curl -sSO https://dl.google.com/cloudagents/add-google-cloud-ops-agent-repo.sh
  sudo bash add-google-cloud-ops-agent-repo.sh --also-install
  sudo systemctl status google-cloud-ops-agent"*"
  ```

- Create an uptime check: Monitoring > Uptime checks > + CREATE UPTIME CHECK
  - Title: Lamp Uptime Check
  - Protocol: HTTP
  - Resource Type: Instance
  - Applies to: Single, lamp-1-vm
  - Path: leave at default
  - Check Frequency: 1 min
  - Leave the other details to default
  - Click Test to verify
  - Create
- Create an alerting policy: Monitoring > Alerting > + CREATE POLICY
  - Select a metric: Disable the `Show only active resources & metrics`
  - VM instance > interface > Network traffic > Apply
  - Threshold position: Above threshold
  - Threshold value: 500
  - Advanced Options > Retest window: 1min
  - Notification Channels > Manage Notification Channels
    - ADD NEW for Email
    - Enter emaill address
    - Save
  - Notification Channels > Select email created above
  - Add a message in documentation
  - Alert name: Inbound Traffic Alert
  - Create Policy
- Create a dashboard and chart: Monitoring > Dashboards > + CREATE DASHBOARD
  - Name: Monitoring LAMP Qwik Start Dashboard
  - Add Chart1: Line > Name: CPU Load > Resource & Metric > Disable `Show only active resources & metrics`
    - VM instance > CPu > CPU load (1m) > Apply
  - Add Chart2: Line > Name: Received Packets > Resource & Metric > Disable `Show only active resources & metrics`
    - VM instance > Instance > Received packets > Apply
- View logs: Navigation menu > Logging > Logs Explorer
  - Resource > VM instance > lamp-1-vm > Apply > Stream logs
- Check what happens when the VM started or stopped
  - Stop the VM, watch the Logs View tab
  - Start the VM, watch the log messages
  - Check uptime check: Monitoring > Uptime checks
  - Check if alerts triggered: Monitoring > Alerting > Check email account added above, a alert email should be received.

## Cloud Functions - Console / Command Line

Cloud Functions是一种用于构建和连接云端服务的serverless执行环境，它以Javascript编写，在Node.js环境中执行。可将Cloud Functions和云基础架构和服务发生的事件进行关联，当事件发生时，函数就会被触发。

|使用场景|说明|
|-|-|
|数据处理/ETL|侦听诸如创建文件、更改文件或移除文件等 Cloud Storage 事件并做出响应。通过您的 Cloud Functions 函数处理图像、执行视频转码、验证和转换数据，以及调用互联网上的任何服务。|
|Webhook|通过简单的 HTTP 触发器，对源自 GitHub、Slack、Stripe 等第三方系统或可发送 HTTP 请求的任意位置的事件进行响应。|
|轻量级 API|从轻量级、松散耦合的小型逻辑开始，编写可迅速构建并可即时扩展的应用。您的函数可以由事件驱动，也可以直接通过 HTTP/S 调用。|
|移动后端|使用 Google 面向应用开发者的移动平台 Firebase，在 Cloud Functions 中编写您的移动后端。侦听 Firebase Analytics、Realtime Database、Authentication 和 Storage 中的事件并做出响应。|
|IoT|想象一下，成千上万的设备向 Cloud Pub/Sub 流式传输数据，并由此启动 Cloud Functions 函数来处理、转换和存储数据。Cloud Functions 能够让您通过一种完全不需要服务器的方式做到这一点。|

### Cloud Console

- Create Function: Navigation menu > Cloud Functions > CREATE FUNCTION
  - Name: GCFunction
  - Trigger: HTTP
  - Memory allocated: default
  - Autoscaling: Maximum number of instances to 5
- Deploy Function:
  - Use default helloWorld function implementation
  - Deploy
- Test Function:
  - Cloud Functions Overview > Test function
  - Triggering event: `{"message":"Hello World!"}`
  - TEST THE FUNCTION
  - Output: Hello World!
  - Logs: status code is 200
- View logs
  - Cloud Function Overview > View logs
  - View log in Query results

### Command Line

```sh
# active cloud shell
gcloud auth list
gcloud config list project
# prepare function code
mkdir gcf_hello_world
cd gcf_hello_world
vim index.js
# create cloud storage bucket
gsutil mb -p [PROJECT_ID] gs://[BUCKET_NAME]
# deploy function to a pub/sub topic named hello_world
gcloud functions deploy helloWorld \
  --stage-bucket [BUCKET_NAME] \
  --trigger-topic hello_world \
  --runtime nodejs8
# verify function
gcloud functions describe helloWorld
# test function
DATA=$(printf 'Hello World!'|base64) && gcloud functions call helloWorld --data '{"data":"'$DATA'"}'
# view logs
gcloud functions logs read helloWorld
```

index.js

```js
/**
* Background Cloud Function to be triggered by Pub/Sub.
* This function is exported by index.js, and executed when
* the trigger topic receives a message.
*
* @param {object} data The event payload.
* @param {object} context The event metadata.
*/
exports.helloWorld = (data, context) => {
const pubSubMessage = data;
const name = pubSubMessage.data
    ? Buffer.from(pubSubMessage.data, 'base64').toString() : "Hello World";
console.log(`My Cloud Function: ${name}`);
};
```

## Cloud Pub/Sub - Console / Command Line / Python

A message service for exchanging event data among applications and services.

- Producer: publishes messages to a Cloud Pub/Sub topic
- Consumer: creates a subscription to that topic. Subscriber either pull messages from a subscription or are configured as webhooks for push subscriptions. Every subscriber must acknowledge each message within a configrable window of time.

### Cloud Console

- Setup Pub/Sub: Navigation menu > Pub/Sub > Topics > CREATE TOPIC
  - Name: MyTopic
  - CREATE TOPIC
- Add subscription: Topic > Create subscription
  - Name: MySub
  - Delivery type: Pull
  - Create
- Publish a message
  - Topics details page > Messages tab > Publish Message
  - Message: Hello World
  - Publish
- View the message

  ```sh
  gcloud pubsub subscriptions pull --auto-ack MySub
  ```

### Command Line

```sh
# active cloud shell
gcloud auth list
# create topic
gcloud pubsub topics create myTopic
gcloud pubsub topics create Test1
gcloud pubsub topics create Test2
# list topic
gcloud pubsub topics list
# delete topic
gcloud pubsub topics delete Test1
gcloud pubsub topics delete Test2
# list topic
gcloud pubsub topics list
# create subscription
gcloud  pubsub subscriptions create --topic myTopic mySubscription
gcloud  pubsub subscriptions create --topic myTopic Test1
gcloud  pubsub subscriptions create --topic myTopic Test2
# list subscriptions
gcloud pubsub topics list-subscriptions myTopic
# delete subscription
gcloud pubsub subscriptions delete Test1
gcloud pubsub subscriptions delete Test2
# list subscriptions
gcloud pubsub topics list-subscriptions myTopic
# publish a single message
gcloud pubsub topics publish myTopic --message "Hello"
gcloud pubsub topics publish myTopic --message "Publisher's name is <YOUR NAME>"
gcloud pubsub topics publish myTopic --message "Publisher likes to eat <FOOD>"
gcloud pubsub topics publish myTopic --message "Publisher thinks Pub/Sub is awesome"
# pull the messages
gcloud pubsub subscriptions pull mySubscription --auto-ack
gcloud pubsub subscriptions pull mySubscription --auto-ack
gcloud pubsub subscriptions pull mySubscription --auto-ack
gcloud pubsub subscriptions pull mySubscription --auto-ack
# no more messages
gcloud pubsub subscriptions pull mySubscription --auto-ack
# publish more messages
gcloud pubsub topics publish myTopic --message "Publisher is starting to get the hang of Pub/Sub"
gcloud pubsub topics publish myTopic --message "Publisher wonders if all messages will be pulled"
gcloud pubsub topics publish myTopic --message "Publisher will have to test to find out"
# pull all messages
gcloud pubsub subscriptions pull mySubscription --auto-ack --limit=3
```

### Python

```sh
# create a virtual environment
apt-get install -y virtualenv
python3 -m venv venv
source venv/bin/activate
pip install --upgrade google-cloud-pubsub
git clone https://github.com/googleapis/python-pubsub.git
cd python-pubsub/samples/snippets
# create topic
echo $GOOGLE_CLOUD_PROJECT
cat publisher.py
python publisher.py -h
python publisher.py $GOOGLE_CLOUD_PROJECT create MyTopic
python publisher.py $GOOGLE_CLOUD_PROJECT list
python subscriber.py $GOOGLE_CLOUD_PROJECT create MyTopic MySub
python subscriber.py $GOOGLE_CLOUD_PROJECT list-in-project
python subscriber.py -h
# publish messages
gcloud pubsub topics publish MyTopic --message "Hello"
gcloud pubsub topics publish MyTopic --message "Publisher's name is <YOUR NAME>"
gcloud pubsub topics publish MyTopic --message "Publisher likes to eat <FOOD>"
gcloud pubsub topics publish MyTopic --message "Publisher thinks Pub/Sub is awesome"
# view messages
python subscriber.py $GOOGLE_CLOUD_PROJECT receive MySub
# Ctrl+C to stop listening
```
