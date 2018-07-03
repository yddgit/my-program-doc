# Quartz

## Download and Install

Download URL: http://www.quartz-scheduler.org/downloads/

Quartz主要的jar包是`quartz-x.x.x.jar`，这是必须要引入的；`quartz-jobs-x.x.x.jar`包含一些内置的job实现，可根据需要引入；还有其依赖的第三方jar，主要是slf4j和c3p0。

Quartz有一个配置文件`quartz.properties`，但并不是必须的，该配置文件需要添加到classpath才会生效。Quartz的详细配置可参考[这里](http://www.quartz-scheduler.org/documentation/quartz-2.2.x/configuration/ "Quartz Configuration Reference")

```properties
# scheduler名字MyScheduler
org.quartz.scheduler.instanceName=MyScheduler
# 线程池大小为3，表示同时最多会有3个job在运行
org.quartz.threadPool.threadCount=3
# 所有job和trigger的数据都存放在内存中
org.quartz.jobStore.class=org.quartz.simpl.RAMJobStore
```

## A Sample Application

如下示例代码，启动了一个Scheduler，添加了一个Job(实现类Hello.class)和Trigger(每5秒运行一次)，然后关闭Scheduler。

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

使用Quartz之前，需要先用`SchedulerFactory`实例化一个`Scheduler`对象。初始化完成后，Scheduler可以start/stand-by/shutdown。

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

Scheduler的生命周期从`SchedulerFactory`类创建其实例开始，到调用其shutdown()方法做为结束。Scheduler一旦实例创建好，就可以添加/移除/查询其中的Job和Trigger，并进行其他任务调度的操作。但是，在调用start()方法之前，Scheduler不会执行任何的Job。

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

Quartz将Job和Trigger这两个概念分开，是有一定的好处的。首先Job可以独立于Trigger进行创建和存储，多个Trigger可以关联同一个Job。另外，这种松散耦合可以使Trigger过期后，可以再次调度保留在Scheduler中的Job，而不需要重新创建。同样也允许修改或替换Trigger而无需重建关联Job。

## Identities

Job和Trigger在注册到Scheduler中时，允许对其(JobKey和TriggerKey)进行分组，这样可以方便管理。如："reporting jobs"、"maintenance jobs"。Job和Trigger的唯一标识就是name+group。

## Jobs and Job Details

Job包含了其具体要执行的业务逻辑，执行Job时，Quartz通过JobDetail对象获取Job的属性信息，JobDetail对象可以通过JobBuilder类的静态方法创建：

```java
import static org.quartz.JobBuilder.*;
```

看一下上文中创建JobDetail的代码：

```java
// define the job and tie it to our HelloJob class
JobDetail job = newJob(HelloJob.class)
    .withIdentity("myJob", "group1") // name "myJob", group "group1"
    .build();
```

只需要传递Job实现类的class对象给newJob方法，然后在每次执行Job时，调度器**创建一个新的Job实例**，执行完之后，该对象实例就会被GC回收。因此每个Job实现类必须有一个无参构造方法，而且不要在Job实现类中保留任何有状态的数据(因为每次执行都会新建实例，这些数据都会丢失)。

如果要在Job执行过程中保存状态数据，需要使用JobDataMap

## JobDataMap

`JobDataMap`中保存的数据可以在Job执行过程中被不同的Job实例获取并使用。`JobDataMap`类是Map接口的实现类，支持原生数据类型的存取操作。示例如下：

```java
// define the job and tie it to our DumbJob class
JobDetail job = newJob(DumbJob.class)
    .withIdentity("myJob", "group1") // name "myJob", group "group1"
    .usingJobData("jobSays", "Hello World!")
    .usingJobData("myFloatValue", 3.141f)
    .build();
```

如下，在Job执行过程中获取JobDataMap中的数据：

```java
public class DumbJob implements Job {

    public DumbJob() { }

    public void execute(JobExecutionContext context) throws JobExecutionException {
        JobKey key = context.getJobDetail().getKey();

        JobDataMap dataMap = context.getJobDetail().getJobDataMap();

        String jobSays = dataMap.getString("jobSays");
        float myFloatValue = dataMap.getFloat("myFloatValue");

        System.err.println("Instance " + key + " of DumbJob says: " + jobSays + ", and val is: " + myFloatValue);
    }
}
```

如果使用持久化的JobStore，需要考虑哪些数据可以传入JobDataMap，因为这些数据会被序列化。显然基本的Java数据类型不会有问题，但除此之外，当更改自定义的类时，必须注意不能破坏兼容性。JDBC-JobStore和JobDataMap只允许原生类型和String的存取，避免了序列化的问题。

如果Job实现类对应JobDataMap的key有对应的set方法(如上例: `setJobSays(String val)`)，那Quartz的默认JobFactory实现类会在实例化Job时，调用这些set方法，将对应的属性传给Job实例，这样就不需要在execute方法中手动获取了。

Trigger也有关联的JobDataMap，这个通常用于一个Job关联多个Trigger时，每个Trigger希望给Job输入不同的数据。

JobDetail和Trigger中的JobDataMap在Job执行过程中会合并，Trigger中的数据会覆盖JobDetail中的同名数据。示例如下：

```java
public class DumbJob implements Job {

    public DumbJob() { }

    public void execute(JobExecutionContext context) throws JobExecutionException {
        JobKey key = context.getJobDetail().getKey();

        JobDataMap dataMap = context.getMergedJobDataMap();  // Note the difference from the previous example

        String jobSays = dataMap.getString("jobSays");
        float myFloatValue = dataMap.getFloat("myFloatValue");
        ArrayList state = (ArrayList)dataMap.get("myStateData");
        state.add(new Date());

        System.err.println("Instance " + key + " of DumbJob says: " + jobSays + ", and val is: " + myFloatValue);
    }
}
```

或者也可以用set方法来接收JobDataMap中的数据：

```java
public class DumbJob implements Job {

    String jobSays;
    float myFloatValue;
    ArrayList state;

    public DumbJob() { }

    public void execute(JobExecutionContext context) throws JobExecutionException {
		JobKey key = context.getJobDetail().getKey();
		JobDataMap dataMap = context.getMergedJobDataMap();  // Note the difference from the previous example
		state.add(new Date());
		System.err.println("Instance " + key + " of DumbJob says: " + jobSays + ", and val is: " + myFloatValue);
    }

    public void setJobSays(String jobSays) {
		this.jobSays = jobSays;
    }

    public void setMyFloatValue(float myFloatValue) {
		this.myFloatValue = myFloatValue;
    }

    public void setState(ArrayList state) {
		this.state = state;
    }
}
```

## Job "Instances"

对于Job实例的理解。可以创建一个Job实现类，然后创建多个JobDetail实例来将不同的数据通过JobDataMap对象传递给Job。如：创建一个`SalesReportJob`实现类，它需要从JobDataMap接收销售人员的名字做为参数来生成该他的报表。这时只需要创建不同的JobDetail实例，如`SalesReportForJoe`/`SalesReportForMike`，通过JobDataMap将"joe"或"mike"参数传递给Job实例即可。

当Trigger被触发，JobDetail会被加载，Job实现类会被`JobFactory`实例化(通过调用class对象的newInstance()方法)，并通过set方法将JobDataMap中匹配的数据传递给Job对象。当然，也可以使用自已的JobFactory实现，如使用IoC容器的依赖注入功能。

通常提到的`Job`指的是JobDetail，Job实现类通常称为`Job类`。

## Job State and Concurrency

有几个注解，可以添加到Job实现类上，以影响Quartz的行为。

1. `@DisallowConcurrentExecution`注解可以添加到Job实现类上，告诉Quartz不要同时执行给定Job定义的多个实例(指向该Job实现类的)。参考上文中提到的例子，假设`SalesReportJob`添加了这个注解，那么同一时刻只能有一个`SalesReportForJoe`的实例在运行，但它可以与`SalesReportForMike`的实例同时执行。

   因此，这里所说的不同实例是以JobDetail为基础的而不是Job实现类。之所以要把这个注解添加到Job实现类上，只是因为Job实现类的代码修改更方便。

2. `@PersistJobDataAfterExecution`注解可以添加到Job实现类上，告诉Quartz在execute()方法成功执行(没有抛异常)后，更新存储的JobDataMap数据。这样，同一个JobDetail下次执行时就能获取到更新后的数据了。与`@DisallowConcurrentExecution`一样，这个注解也是以JobDetail为基础区分的，而不是Job实现类。

   如果要使用`@PersistJobDataAfterExecution`，强烈建议同时添加`@DisallowConcurrentExecution`注解，以避免多个实例同时更新Job数据。

## Other Attributes Of Jobs

1. Durability。如果Job是非持久的，那么当没有关联活动的Trigger时该Job就会被删除。换句话说，就是非持久Job的生命周期是与其绑定的Trigger相关的。

2. RequestsRecovery。如果Job是"requests recovery"的，那么当scheduler非正常关闭后再次启动时，该Job会重新执行。此时，`JobExecutionContext.isRecovering()`方法会返回true。

## JobExecutionException

Job实现类的execute()方法只能抛出JobExecutionException。因此可以将execute()方法的所有代码包到try-catch块中，参考[Quartz Javadoc](http://www.quartz-scheduler.org/api/2.2.1/index.html)中`org.quartz.JobExecutionException`类的API，可以为调度器提供有关如何处理异常的各种指令。