# Quartz

## Download and Install

Download URL: http://www.quartz-scheduler.org/downloads/

Quartz主要的jar包是`quartz-x.x.x.jar`，这是必须要引入的；`quartz-jobs-x.x.x.jar`包含一些内置的job实现，可根据需要引入；还有其依赖的第三方jar，主要是slf4j和c3p0。

Quartz的配置文件`quartz.properties`并不必须的，该配置文件需要添加到classpath才会生效。Quartz的详细配置可参考[这里](http://www.quartz-scheduler.org/documentation/quartz-2.2.x/configuration/ "Quartz Configuration Reference")

```properties
# scheduler名字MyScheduler
org.quartz.scheduler.instanceName=MyScheduler
# 线程池大小为3，表示同时最多会有3个job在运行
org.quartz.threadPool.threadCount=3
# 所有job和trigger的数据都存放在内存中
org.quartz.jobStore.class=org.quartz.simpl.RAMJobStore
```

## A Sample Application

如下示例代码，启动了一个Scheduler，添加了一个job(实现类Hello.class)和trigger(每40秒运行一次)，然后关闭了Scheduler。

```java
public class QuartzSample {

	public static void main(String[] args) throws InterruptedException {
        try {
            // Grab the Scheduler instance from the Factory
            Scheduler scheduler = StdSchedulerFactory.getDefaultScheduler();

            // and start it off
            scheduler.start();

            // define the job and tie it to our HelloJob class
            JobDetail job = newJob(HelloJob.class)
                .withIdentity("job1", "group1")
                .build();

            // Trigger the job to run now, and then repeat every 40 seconds
            Trigger trigger = newTrigger()
                .withIdentity("trigger1", "group1")
                .startNow()
                    .withSchedule(simpleSchedule()
                        .withIntervalInSeconds(5)
                        .repeatForever())            
                .build();

            // Tell quartz to schedule the job using our trigger
            scheduler.scheduleJob(job, trigger);

            // allow some time for the job to be triggered and executed
            Thread.sleep(10000);

            scheduler.shutdown();

        } catch (SchedulerException se) {
            se.printStackTrace();
        }
    }

	public static class HelloJob implements Job {
		@Override
		public void execute(JobExecutionContext context) throws JobExecutionException {
			System.out.println("HelloJob is executed.");
		}
	}
}
```

## Using Quartz

使用Scheduler之前，需要先使用`SchedulerFactory`实例化一个`Scheduler`对象。初始化完成后，Scheduler可以start/stand-by/shutdown。

在Scheduler启动之前，job是不会执行的，如果Scheduler处于paused状态也不会执行job。

## Quartz API

| 类名 | 描述 |
|------|-----|
| Scheduler | 操作调度器的主要API |
| Job | 需要用户实现该接口的execute方法，包含job具体的执行逻辑 |
| JobDetail | 用来定义job的实例 |
| Trigger | 定义job的执行时间表 |
| JobBuilder | 用来定义和创建JobDetail实例 |
| TriggerBuilder | 用于定义和创建Trigger实例 |

Scheduler的生命周期从通过`SchedulerFactory`类创建其实例开始，到调用其shutdown()方法做为结束。Scheduler一旦实例创建好，就可以添加/移除/查询其中的Job和Trigger，并进行其他任务调度的操作。但是，在调用start()方法之前，Scheduler不会执行任何的Job。

Quartz提供了很多builder类，支持链式编程，如`JobBuilder`/`TriggerBuilder`/`SimpleScheduleBuilder`等，可以使用如下import语句引入:

```java
import static org.quartz.JobBuilder.*;
import static org.quartz.SimpleScheduleBuilder.*;
import static org.quartz.CronScheduleBuilder.*;
import static org.quartz.CalendarIntervalScheduleBuilder.*;
import static org.quartz.TriggerBuilder.*;
import static org.quartz.DateBuilder.*;
```

这里有很多ScheduleBuilder用于创建不同类型的Scheduler。`DateBuilder`包含一些简便的方法用于创建特定时点的`java.util.Date`实例。如：下一个偶数小时或者下一个整点时刻等。

## Jobs and Triggers

一个Job类需要实现Job接口，只需要实现其execute方法

```java
package org.quartz;

public interface Job {
    public void execute(JobExecutionContext context) throws JobExecutionException;
}
```

当Job的触发器被触发，Scheduler的工作线程就会调用一次Job的execute()方法，并传递一个JobExecutionContext对象。这个对象包含执行当前Job的Scheduler对象、触发本次Job执行的Trigger对象、Job关联的JobDetail对象以及其他数据。

JobDetail对象是用户在添加Job到Scheduler时创建的，包含Job的一些属性设置，以及一个`JobDataMap`对象，它可以用来存储一些需要传递给Job运行实例的状态信息。JobDetail本质上是定义了一个Job运行实例。

Trigger用来触发Job的执行。Trigger也有一个关联的`JobDataMap`，可用于将参数传递给由特定Trigger触发的Job实例。Quartz提供了很多不同的Trigger类型，但最常用的是`SimpleTrigger`和`CronTrigger`。

SimpleTrigger可用于一次执行的场景，即只需要Job在特定时刻执行一次。或者是在特定时刻开始执行，重复N次，每次执行保持固定时间间隔的场合。
CronTrigger用于基于日历时间的任务调度场景。

Quartz将Job和Trigger这两个概念分开，是有一定的好处的。首先Job可以独立于Trigger进行创建和存储，多个Trigger可以关联同一个Job。另外，这种松散耦合可以使用Trigger过期后，可以再次调度保留在Scheduler中的Job，而不需要重新创建。同样也允许修改或替换Trigger而无需重建关联Job。

## Identities

Job和Trigger在注册到Scheduler中时，允许对其(JobKey和TriggerKey)进行分组，这样可以方便管理。如："reporting jobs"、"maintenance jobs"。Job和Trigger的唯一标识就是name+group。

