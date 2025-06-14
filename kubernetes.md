# Kubernetes

## 安装

* Docker Desktop for Windows: [k8s-for-docker-desktop](https://github.com/AliyunContainerService/k8s-for-docker-desktop "k8s-for-docker-desktop")
* Minikube: [minikube start](https://minikube.sigs.k8s.io/docs/start/ "minikube start")

> [Kubernetes的版本支持策略](https://kubernetes.io/zh/docs/setup/release/version-skew-policy/ "Kubernetes的版本支持策略")，版本号格式遵循 [Semantic Versioning](https://semver.org/ "Semantic Versioning") 规则

## 概念

Kubernetes是一个生产级别的容器编排系统，集群包含两种类型的资源

* Master: 调度整个集群，例如调度应用、维护应用的所需状态、应用扩容以及推送更新
* Nodes: 负责运行应用，是一个虚拟机或物理机，充当工作机器的角色。处理生产级流量**至少**应具有三个Node。Node使用master暴露的Kubernetes API与Master通信。每个Node至少运行
  * Kubelet，负责Master和Node之间的通信，管理Pod和机器上运行的容器
  * 容器运行时，如Docker。负责从仓库中提取镜像，解压容器及运行应用程序

为了在Kubernetes上部署应用，需要创建Kubernetes Deployment配置

* Deployment: 指挥Kubernetes如何创建和更新应用程序的实例

创建Deployment后，Kubernetes master将应用程序实例调度到集群中的各个节点上。Deployment控制器会持续监视这些实例，如果节点关闭或删除，控制器会将实例替换为另一个节点上的实例(这提供了一种自我修复机制还解决机器故障维护问题)

在创建Deployment时，Kubernetes添加了个Pod来托管应用实例

* Pod: 表示一组一个或多个应用程序容器(如Docker)和这些容器的一些共享资源：共享存储、网络、有关每个容器如何运行的信息，如镜像版本、特定端口等

Pod为特定于应用程序“逻辑主机”建模，并可以包含相对紧耦合的不同应用容器，**Pod中的容器共享IP地址和端口**，始终位于同一位置，共同调度，并在同一工作节点上的共享上下文中运行。Pod是Kubernetes平台上的原子单元。

当创建Deployment时，会创建包含容器的Pod(而不是直接创建容器)。Pod总是运行在Node上(工作节点)，每个Node由Master管理，Master自动处理Node上调度Pod，Master的自动调度会考量每个Node上的可用资源。

`kubectl`常见操作:

1. kubectl get - 列出资源
2. kubectl describe - 显示有关资源的详细信息
3. kubectl logs - 打印pod和其中窗口的日志
4. kubectl exec - 在pod容器上执行命令

Pod是转瞬即逝的，可以会丢失或重建，每个Pod都有一个唯一的IP地址。此时需要一种自动协调Pod之间的变更，以便应用程序保持运行

* Service: 定义了Pod的逻辑集和访问Pod的协议，使用Pod之间的松耦合成为可能。Service下的一组Pod通常由**LabelSelector**来标记

尽管每个Pod都有一个唯一的IP，但如果没有Service，这些IP不会暴露在集群外部。Service允许应用程序接收流量，也可以用在ServiceSpec标记type的方式暴露

- ClusterIP(默认) - 在集群的内部IP上公开Service，这使得Service只能从集群内访问
- NodePort - 使用NAT在集群中每个选定Node的相同端口上公开Service。使用`<NodeIP>:<NodePort>`从集群外部访问Service，是ClusterIP的超集
- LoadBalancer - 在当前云中创建一个外部负载均衡器，并为Service分配一个固定外部IP，是NodePort的超集
- ExternalName - 通过返回带有该名称的CNAME记录，使用任意名称(由spec中的externalName指定)公开Service。不使用代理，这种类型需要kube-dns v1.7+

Service通过一组Pod路由通信，它允许Pod死亡、复制而不影响应用程序，匹配Pod使用标签(Label)和选择器(Selector)。标签可以以多种方式使用

- 指定用于开发、测试和生产的对象
- 嵌入版本标签
- 使用label将对象进行分类

扩缩容通过改变Deployment中的副本数量来实现

- 扩展将创建新的Pod并将调度请求分配到有可用资源的节点上
- 收缩会将Pod数量减少至所需的状态
- 将Pod数量收缩到0也是可以的，这会终止deployment上所有已经部署的Pod

滚动更新允许使用新的实例逐步更新Pod实例，零停机。允许以下操作

- 将应用程序从一个环境提升到另一个环境
- 回滚到以前的版本
- 持续集成和持续交付应用程序，无需停机
- 如果deployment是公开的，则服务将仅在更新期间对可用的pod的进行负载均衡

## 命令

```bash
# 查看version, 包含client和server
kubectl version
# 查看集群基本信息
kubectl cluster-info
# 查看集群中的Node
kubectl get nodes

# 获得命令帮助
kubectl get nodes --help
# 创建一个deployment
kubectl create deployment hello-kubernetes --image=ydd1226/hello-kubernetes:1.0
# 查看deployment
kubectl get deployments

# 访问部署的应用
# 启动proxy
kubectl proxy
# 验证proxy启动
curl http://localhost:8001/version
# 获得pod名字
export POD_NAME=$(kubectl get pods -o go-template --template '{{range .items}}{{.metadata.name}}{{"\n"}}{{end}}')
echo Name of the Pod: $POD_NAME
# 查看pod信息
curl http://localhost:8001/api/v1/namespaces/default/pods/$POD_NAME/

# 查看pod
kubectl get pods
# 查看pod详细信息
kubectl describe pods
# 访问pod里应用的地址: localhost:8080/greeting
curl http://localhost:8001/api/v1/namespaces/default/pods/$POD_NAME:8080/proxy/greeting
# 查看容器日志
kubectl logs $POD_NAME
# 在容器中执行命令(如果Pod中只有一个容器则容器名称可以省略)
kubectl exec $POD_NAME -c hello-kubernetes env
kubectl exec $POD_NAME -- env
# 在容器里启动一个bash session
kubectl exec -ti $POD_NAME -- bash

# 查看service
kubectl get services
# 以NodePort参数创建一个service
kubectl expose deployment/hello-kubernetes --type="NodePort" --port 8080
# 查看service暴露的端口
kubectl describe services/hello-kubernetes
# 获得NodePort
export NODE_PORT=$(kubectl get services/hello-kubernetes -o go-template='{{(index .spec.ports 0).nodePort}}')
echo NODE_PORT=$NODE_PORT
# 访问应用的地址: localhost:8080/greeting
curl localhost:$NODE_PORT/greeting

# 查看deployment上的label
kubectl describe deployments
# 使用label查询pod
kubectl get pods -l app=hello-kubernetes
# 使用label查询service
kubectl get services -l app=hello-kubernetes
# 给pod/service/deployment加label
kubectl label pods $POD_NAME version=v1
# 查看添加后的label
kubectl describe pods $POD_NAME
# 使用新添加的label查询pod
kubectl get pods -l version=v1

# 使用label删除service
kubectl delete services -l app=hello-kubernetes
# 确认service是否删除成功
kubectl get services
# 之前配置的NodePort也访问不了了
curl localhost:$NODE_PORT/greeting
# 但Pod里的容器仍在正常运行
kubectl exec -it $POD_NAME -- curl localhost:8080/greeting
curl http://localhost:8001/api/v1/namespaces/default/pods/$POD_NAME:8080/proxy/greeting

# 查看已有的deployment
kubectl get deployments
# 查看replicaSet
kubectl get rs
# 扩容至4个副本
kubectl scale deployments/hello-kubernetes --replicas=4
# 查看扩容过程
kubectl get deployments
kubectl get pods -o wide
# 以上变更会记录到deployment的event log里
kubectl describe deployments/hello-kubernetes

# 验证Load Balancing
kubectl expose deployment/hello-kubernetes --type="NodePort" --port 8080
kubectl describe services/hello-kubernetes
export NODE_PORT=$(kubectl get services/hello-kubernetes -o go-template='{{(index .spec.ports 0).nodePort}}')
echo NODE_PORT=$NODE_PORT
# 访问应用的地址: localhost:8080/greeting
curl localhost:$NODE_PORT/greeting

# 收缩到2个副本
kubectl scale deployments/hello-kubernetes --replicas=2
# 查看deployment的READY和AVAILABLE
kubectl get deployments
# Pod数量也收缩到了2
kubectl get pods -o wide

# 查看deployment
kubectl get deployments
# 查看pod
kubectl describe pods
# 更新应用镜像
kubectl set image deployments/hello-kubernetes hello-kubernetes=ydd1226/hello-kubernetes:1.1
# 观察Pod停止启动的过程
kubectl get pods
# 确认应用服务的状态
curl localhost:$NODE_PORT/greeting
# 确认滚动更新的状态
kubectl rollout status deployments/hello-kubernetes
# 确认镜像已经更新
kubectl describe pods

# 先使用一个不存在的镜像升级应用
kubectl set image deployments/hello-kubernetes hello-kubernetes=ydd1226/hello-kubernetes:1.2
# 查看pod状态, Status显示ImagePullBackOff
kubectl get pods
# 执行回滚操作
kubectl rollout undo deployments/hello-kubernetes
# 查看pod状态
kubectl get pods
# 查看pod详情
kubectl describe pods
```

## 完整安装

可以用WMWare Workstation Pro安装4台虚拟机来安装

- k8sm1
- k8sm2
- k8sm3
- k8sn1

系统镜像使用OSBoxes提供的 [Ubuntu 24.04](https://www.osboxes.org/ubuntu-server/#ubuntu-server-24-04-info "Ubuntu 24.04") 的系统

### 系统初始化

> OSBoxes提供的系统镜像账号和密码都是：osboxes/osboxes.org

```sh
# 修改源
sudo sed -i "s@http://archive.ubuntu.com@https://mirrors.aliyun.com@g" /etc/apt/sources.list.d/ubuntu.sources
sudo sed -i "s@http://security.ubuntu.com@https://mirrors.aliyun.com@g" /etc/apt/sources.list.d/ubuntu.sources
# 安装ping/telnet/vim
sudo apt-get update
sudo apt-get install -y iputils-ping telnet vim openssh-server netcat-openbsd
sudo systemctl status ssh
sudo systemctl enable ssh
sudo systemctl start ssh

# 查看hostname
hostnamectl
# 设置新主机名
sudo hostnamectl set-hostname k8sm1
# 修改etc/hosts(中的主机名)
sudo vim /etc/hosts

# 查看IP
ip addr
# 修改为静态IP(改为和VMWare NAT Network相同网段的IP)
sudo vim /etc/netplan/50-cloud-init.yaml
# 使修改生效
sudo netplan apply
# 验证网络
ping -c 4 k8s.io

# 重启待命
sudo reboot

```

```sh
# /etc/hosts
127.0.0.1 localhost
127.0.0.1 k8sm1

......
```

```yml
# /etc/netplan/50-cloud-init.yaml
network:
  ethernets:
    ens33:
      dhcp4: no
      addresses:
        - 192.168.80.10/24
      routes:
        - to: 0.0.0.0/0
          via: 192.168.80.2
      nameservers:
        addresses: [8.8.8.8, 1.1.1.1]
  version: 2
```
安装Docker
```sh
# step 1: 安装必要的一些系统工具
sudo apt-get update
sudo apt-get install ca-certificates curl gnupg

# step 2: 信任 Docker 的 GPG 公钥
sudo install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://mirrors.aliyun.com/docker-ce/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg

# Step 3: 写入软件源信息
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://mirrors.aliyun.com/docker-ce/linux/ubuntu \
  "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# Step 4: 安装Docker
sudo apt-get update
sudo apt-get install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# Step 5: Docker开机启动
sudo systemctl enable docker
sudo systemctl status docker
sudo systemctl start docker

# Step 6: 添加当前用户到docker组
sudo usermod -aG docker osboxes
```

### 安装k8s相关软件

每台机器/etc/hosts/增加如下内容，确保互相可以用主机名访问到
```
192.168.80.10 k8sm1
192.168.80.20 k8sm2
192.168.80.30 k8sm3
192.168.80.40 k8sn1
```
可能需要为机器配置代理
```sh
cat <<EOF | sudo tee -a /etc/profile
export no_proxy="192.168.*,172.31.*,172.30.*,172.29.*,172.28.*,172.27.*,172.26.*,172.25.*,172.24.*,172.23.*,172.22.*,172.21.*,172.20.*,172.19.*,172.18.*,172.17.*,172.16.*,10.*,127.*,localhost,<local>"
export https_proxy="http://192.168.80.1:10808"
export http_proxy="http://192.168.80.1:10808"
EOF
```
启用 IPv4 数据包转发，加载相关系统模块
```sh
# 设置模块自动加载
cat <<EOF | sudo tee /etc/modules-load.d/k8s.conf
br_netfilter
EOF
# 立即加载
sudo systemctl restart systemd-modules-load.service

# 设置所需的 sysctl 参数，参数在重新启动后保持不变
cat <<EOF | sudo tee /etc/sysctl.d/k8s.conf
net.bridge.bridge-nf-call-iptables  = 1
net.ipv4.ip_forward                 = 1
net.bridge.bridge-nf-call-ip6tables = 1
EOF
# 应用 sysctl 参数而不重新启动
sudo sysctl --system
# 验证
sysctl net.ipv4.ip_forward
sysctl net.bridge.bridge-nf-call-iptables
sysctl net.bridge.bridge-nf-call-ip6tables
```
因为使用Docker Engine作为容器运行时，所以需要[安装cri-dockerd](https://mirantis.github.io/cri-dockerd/usage/install-manually/ "Install Manually")
```sh
wget https://github.com/Mirantis/cri-dockerd/releases/download/v0.4.0/cri-dockerd-0.4.0.amd64.tgz
wget https://raw.githubusercontent.com/Mirantis/cri-dockerd/master/packaging/systemd/cri-docker.service
wget https://raw.githubusercontent.com/Mirantis/cri-dockerd/master/packaging/systemd/cri-docker.socket
tar -xzf cri-dockerd-0.4.0.amd64.tgz
cd cri-dockerd
sudo install -o root -g root -m 0755 cri-dockerd /usr/local/bin/cri-dockerd
cd ..
sudo install cri-docker.service /etc/systemd/system
sudo install cri-docker.socket /etc/systemd/system
sudo sed -i -e 's,/usr/bin/cri-dockerd,/usr/local/bin/cri-dockerd,' /etc/systemd/system/cri-docker.service
sudo systemctl daemon-reload
sudo systemctl enable cri-docker.service cri-docker.socket
sudo systemctl start cri-docker.service cri-docker.socket
sudo systemctl status cri-docker.service cri-docker.socket
```
检查操作系统版本，K8S支持[LTS内核](https://www.kernel.org/category/releases.html "LTS内核")
```sh
uname -r
```
确保每个节点MAC地址和product_uuid的唯一性
```sh
ip link
sudo cat /sys/class/dmi/id/product_uuid
```
关闭Swap分区
```
sudo swapoff -a
sudo sed -ri 's/.*swap.*/#&/' /etc/fstab
```
检查网络适配器和所需端口（确保端口**没有**被占用）
```sh
nc 127.0.0.1 6443 -zv -w 2
```
安装 kubeadm、kubelet 和 kubectl([阿里云镜像源](https://developer.aliyun.com/mirror/kubernetes "阿里云镜像源"))
```sh
sudo apt-get update
sudo apt-get install -y apt-transport-https ca-certificates curl gpg
# 如果 `/etc/apt/keyrings` 目录不存在，则应在 curl 命令之前创建它，请阅读下面的注释。
# sudo mkdir -p -m 755 /etc/apt/keyrings
curl -fsSL https://pkgs.k8s.io/core:/stable:/v1.33/deb/Release.key | sudo gpg --dearmor -o /etc/apt/keyrings/kubernetes-apt-keyring.gpg
# 此操作会覆盖 /etc/apt/sources.list.d/kubernetes.list 中现存的所有配置。
echo 'deb [signed-by=/etc/apt/keyrings/kubernetes-apt-keyring.gpg] https://pkgs.k8s.io/core:/stable:/v1.33/deb/ /' | sudo tee /etc/apt/sources.list.d/kubernetes.list
sudo apt-get update
sudo apt-get install -y kubelet kubeadm kubectl
sudo apt-mark hold kubelet kubeadm kubectl
```
配置keepalived和haproxy。这里选择部署在k8s集群的3个master节点，生产环境应该在单独的机器上部署。为了防止端口冲突，haproxy的端口指定8443。如果是单独机器部署，haproxy的端口就可以指定6443。
```sh
sudo apt-get install -y keepalived haproxy
# 配置keepalived
cd /etc/keepalived/
sudo cp keepalived.conf.sample keepalived.conf
cat <<EOF | sudo tee /etc/keepalived/notify-master.sh
#!/bin/bash
logger "[Keepalived] VIP acquired on $(hostname), restarting haproxy..."
systemctl restart haproxy
EOF
sudo chmod +x /etc/keepalived/notify-master.sh
# 仅在k8sm1执行（节点之间state和priority不同）
cat <<EOF | sudo tee /etc/keepalived/keepalived.conf
! Configuration File for keepalived

vrrp_script chk_haproxy {
    script "pidof haproxy"
    interval 2
    weight 2
}

vrrp_instance VI_1 {
    state MASTER
    interface ens33
    virtual_router_id 51
    priority 100
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        192.168.80.100
    }
    track_script {
        chk_haproxy
    }
    notify_master "/etc/keepalived/notify-master.sh"
}
EOF
# 仅在k8sm2执行（节点之间state和priority不同）
cat <<EOF | sudo tee /etc/keepalived/keepalived.conf
! Configuration File for keepalived

vrrp_script chk_haproxy {
    script "pidof haproxy"
    interval 2
    weight 2
}

vrrp_instance VI_1 {
    state BACKUP
    interface ens33
    virtual_router_id 51
    priority 90
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        192.168.80.100
    }
    track_script {
        chk_haproxy
    }
    notify_master "/etc/keepalived/notify-master.sh"
}
EOF
# 仅在k8sm3执行（节点之间state和priority不同）
cat <<EOF | sudo tee /etc/keepalived/keepalived.conf
! Configuration File for keepalived

vrrp_script chk_haproxy {
    script "pidof haproxy"
    interval 2
    weight 2
}

vrrp_instance VI_1 {
    state BACKUP
    interface ens33
    virtual_router_id 51
    priority 80
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        192.168.80.100
    }
    track_script {
        chk_haproxy
    }
    notify_master "/etc/keepalived/notify-master.sh"
}
EOF
# 启动keepalived
sudo systemctl enable keepalived
sudo systemctl start keepalived
# 配置haproxy
cat <<EOF | sudo tee /etc/haproxy/haproxy.cfg
global
    log /dev/log    local0
    log /dev/log    local1 notice
    maxconn 4096
    user haproxy
    group haproxy
    daemon

defaults
    log     global
    mode    tcp
    option  tcplog
    timeout connect 10s
    timeout client  1m
    timeout server  1m

frontend k8s-api
    bind 192.168.80.100:8443
    mode tcp
    default_backend k8s-api-backend

backend k8s-api-backend
    mode tcp
    balance roundrobin
    option tcp-check
    server k8sm1 192.168.80.10:6443 check
    server k8sm2 192.168.80.20:6443 check
    server k8sm3 192.168.80.30:6443 check
EOF
# 仅在k8sm1执行（因为当前VIP在k8sm1上，之后随着VIP飘移，keepalived的notify_master脚本会自动把haproxy拉起来）
sudo systemctl start haproxy
```
初始化集群（k8sm1）
```sh
# 查看镜像
kubeadm config images list
# 提前拉取镜像
docker pull registry.aliyuncs.com/google_containers/kube-apiserver:v1.33.1
docker pull registry.aliyuncs.com/google_containers/kube-controller-manager:v1.33.1
docker pull registry.aliyuncs.com/google_containers/kube-scheduler:v1.33.1
docker pull registry.aliyuncs.com/google_containers/kube-proxy:v1.33.1
docker pull k8s.m.daocloud.io/coredns/coredns:v1.12.0
docker pull registry.aliyuncs.com/google_containers/pause:3.10
docker pull registry.aliyuncs.com/google_containers/etcd:3.5.21-0

docker tag registry.aliyuncs.com/google_containers/kube-apiserver:v1.33.1 registry.k8s.io/kube-apiserver:v1.33.1
docker tag registry.aliyuncs.com/google_containers/kube-controller-manager:v1.33.1 registry.k8s.io/kube-controller-manager:v1.33.1
docker tag registry.aliyuncs.com/google_containers/kube-scheduler:v1.33.1 registry.k8s.io/kube-scheduler:v1.33.1
docker tag registry.aliyuncs.com/google_containers/kube-proxy:v1.33.1 registry.k8s.io/kube-proxy:v1.33.1
docker tag k8s.m.daocloud.io/coredns/coredns:v1.12.0 registry.k8s.io/coredns/coredns:v1.12.0
docker tag registry.aliyuncs.com/google_containers/pause:3.10 registry.k8s.io/pause:3.10
docker tag registry.aliyuncs.com/google_containers/etcd:3.5.21-0 registry.k8s.io/etcd:3.5.21-0

docker rmi registry.aliyuncs.com/google_containers/kube-apiserver:v1.33.1
docker rmi registry.aliyuncs.com/google_containers/kube-controller-manager:v1.33.1
docker rmi registry.aliyuncs.com/google_containers/kube-scheduler:v1.33.1
docker rmi registry.aliyuncs.com/google_containers/kube-proxy:v1.33.1
docker rmi k8s.m.daocloud.io/coredns/coredns:v1.12.0
docker rmi registry.aliyuncs.com/google_containers/pause:3.10
docker rmi registry.aliyuncs.com/google_containers/etcd:3.5.21-0
# 初始化集群
sudo kubeadm init --control-plane-endpoint "192.168.80.100:8443" --upload-certs --pod-network-cidr=10.244.0.0/16 --cri-socket unix:///var/run/cri-dockerd.sock
# 使用集群：非root用户
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown $(id -u):$(id -g) $HOME/.kube/config
# 使用集群：root用户
export KUBECONFIG=/etc/kubernetes/admin.conf
# 安装网络插件
docker pull ghcr.nju.edu.cn/flannel-io/flannel:v0.27.0
docker pull ghcr.nju.edu.cn/flannel-io/flannel-cni-plugin:v1.7.1-flannel1

docker tag ghcr.nju.edu.cn/flannel-io/flannel:v0.27.0 ghcr.io/flannel-io/flannel:v0.27.0
docker tag ghcr.nju.edu.cn/flannel-io/flannel-cni-plugin:v1.7.1-flannel1 ghcr.io/flannel-io/flannel-cni-plugin:v1.7.1-flannel1

docker rmi ghcr.nju.edu.cn/flannel-io/flannel:v0.27.0
docker rmi ghcr.nju.edu.cn/flannel-io/flannel-cni-plugin:v1.7.1-flannel1

kubectl apply -f https://github.com/flannel-io/flannel/releases/latest/download/kube-flannel.yml
```
添加k8sm2和k8sm3（先参考k8sm1拉取镜像）
```sh
# 注意--cri-socket参数需要手动添加
# 如果kubeadm init的时候忘记添加--upload-certs参数，可以用如下命令手动upload（两小时内有效），然后把结果添加到--certificate-key中
# sudo kubeadm init phase upload-certs --upload-certs
sudo kubeadm join 192.168.80.100:8443 --token nxpzz3.ubnl4hs5oin0q23k \
  --discovery-token-ca-cert-hash sha256:02e21504b6c425c2d95bd4532c495cb9beca16b66d9dd4a4a7e54d3b7bb84f12 \
  --control-plane \
  --certificate-key 64ac8cce8d883d916759f1c8c4ef933a5d39c6f89502d8887b4a43ebba54cc49 \
  --cri-socket unix:///var/run/cri-dockerd.sock
# 使用集群：非root用户
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown $(id -u):$(id -g) $HOME/.kube/config
```
添加k8sn1（先参考k8sm1拉取镜像）
```sh
# 注意--cri-socket参数需要手动添加
sudo kubeadm join 192.168.80.100:8443 --token nxpzz3.ubnl4hs5oin0q23k \
  --discovery-token-ca-cert-hash sha256:02e21504b6c425c2d95bd4532c495cb9beca16b66d9dd4a4a7e54d3b7bb84f12 \
  --cri-socket unix:///var/run/cri-dockerd.sock
```
在本地电脑管理集群
```sh
# ================在WSL里安装kubectl================
sudo apt-get update
sudo apt-get install -y apt-transport-https ca-certificates curl gpg
# 如果 `/etc/apt/keyrings` 目录不存在，则应在 curl 命令之前创建它，请阅读下面的注释。
# sudo mkdir -p -m 755 /etc/apt/keyrings
curl -fsSL https://pkgs.k8s.io/core:/stable:/v1.33/deb/Release.key | sudo gpg --dearmor -o /etc/apt/keyrings/kubernetes-apt-keyring.gpg
# 此操作会覆盖 /etc/apt/sources.list.d/kubernetes.list 中现存的所有配置。
echo 'deb [signed-by=/etc/apt/keyrings/kubernetes-apt-keyring.gpg] https://pkgs.k8s.io/core:/stable:/v1.33/deb/ /' | sudo tee /etc/apt/sources.list.d/kubernetes.list
sudo apt-get update
sudo apt-get install -y kubectl
sudo apt-mark hold kubectl
mkdir -p $HOME/.kube
# ================在宿主机 C:\Users\user1\Desktop================
scp osboxes@192.168.80.10:/home/osboxes/.kube/config .
# ================在WSL里================
mv /mnt/c/Users/user1/Desktop/config $HOME/.kube/
chown $(id -u):$(id -g) $HOME/.kube/config
chmod 644 $HOME/.kube/config
# 查看集群节点
kubectl get nodes
# 启动代理API服务器
kubectl proxy
```
问题诊断可能会用到的命令（查看是否有异常Exited的容器）
```sh
# 查看网络插件是否正常启动
kubectl get pods -n kube-flannel -o wide
# 查看pod控制台日志
kubectl logs -n kube-flannel <pod-name>
# 查看pod详情
kubectl describe pod -n kube-flannel <pod-name>
# 修复系统参数后重启网络插件
kubectl delete pod -n kube-flannel -l app=flannel
# 查看apiserver是否正常启动（etcd集群异常会导致apiserver启动异常或自动重启，所以如果不是单独的etcd集群，那添加master节点时，间隔时间最好久一点）
kubectl -n kube-system get pods -o wide | grep apiserver
```

## 卸载集群

移除worker节点
```sh
# 在k8sm1上执行
kubectl drain k8sn1 --delete-emptydir-data --force --ignore-daemonsets
# 在k8sn1上执行
sudo kubeadm reset --cri-socket unix:///var/run/cri-dockerd.sock
# 在k8sm1上执行
kubectl delete node k8sn1
```
移除其他master节点
```sh
# 在k8sm1上执行
kubectl drain k8sm3 --delete-emptydir-data --force --ignore-daemonsets
kubectl drain k8sm2 --delete-emptydir-data --force --ignore-daemonsets
# 在k8sm3和k8sm2上分别执行
sudo kubeadm reset --cri-socket unix:///var/run/cri-dockerd.sock
# 在k8sm1上执行
kubectl delete node k8sm3
kubectl delete node k8sm2
```
重置集群最后一个master节点
```sh
# 在k8sm1上执行
kubectl drain k8sm1 --delete-emptydir-data --force --ignore-daemonsets
sudo kubeadm reset --cri-socket unix:///var/run/cri-dockerd.sock
```