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