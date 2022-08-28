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