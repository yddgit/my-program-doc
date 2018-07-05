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

对于Job实例的理解。可以创建一个Job实现类，然后创建多个JobDetail实例来将不同的数据通过JobDataMap对象传递给Job。如：创建一个`SalesReportJob`实现类，它需要从JobDataMap接收销售人员的名字做为参数来生成他的报表。这时只需要创建不同的JobDetail实例，如`SalesReportForJoe`/`SalesReportForMike`，通过JobDataMap将"joe"或"mike"参数传递给Job实例即可。

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

## Common Trigger Attributes

Trigger也有很多可以自定义的功能，Quartz提供了多种不同类型的Trigger用于不同的调度场景。Trigger有一些共同的属性，如：`TriggerKey`，这些属性可以在创建Trigger实例时使用`TriggerBuilder`类的方法进行设置：

* `jobKey`指定了Trigger被触发时要执行的Job实例的标识。
* `startTime`指定了Trigger第一次被触发生效的时间。是一个`java.util.Date`对象。有些Trigger是启动就被触发了，有些是启动时标记了执行时间，这意味着可以通过日程安排存储Trigger。
* `endTime`指定了Trigger何时失效。如果一个Trigger指定为每月5天执行，endTime设为7月1日，那么Trigger最后一次被触发的时间将是6月5日。

## Trigger Priority

有时候，有多个Trigger在同一时间被触发，但此时Quartz没有足够的资源来处理(如线程池空闲线程数不足)，就需要对这些Trigger进行优先级排序。创建Trigger对象时可以指定其`priority`属性，如果有N个Trigger同时触发，但只有Z个工作线程可用，那么优先级最高的Z个Trigger就会被优先执行。如果不设置，`priority`默认值是5，`priority`的值可以是任意整数，正值或负值都可以。

**Note**：优先级比较仅在同一时刻触发的Trigger间发生，10:59的Trigger永远会在11:00的Trigger之前被触发。

**Note**：如果检测到Job是"require recovery"的，那么恢复时的优先级与原优先级一致。

## Trigger Misfire Instructions

"misfire instruction"是Trigger的另一个重要属性。一个持久存储的Trigger错过了触发时间，可能是由于调度器关闭了，或者线程池没有足够的线程去执行Job了。

不同类型的Trigger有不同的"misfire instruction"，默认使用一个"smart policy"的策略，它会基于Trigger的类型和配置动态调整。调度器启动时，它首先查询所有misfire的Trigger，根据各自misfire的配置更新这些Trigger。

如果在自己的项目中使用Quartz，需要熟悉自己使用的Trigger的"misfire instruction"，可以参考Quartz的Javadoc和官方文档。

## Triggers with Calendar

Quartz的Calendar对象在Trigger被定义和存入调度器时被关联到Trigger上。Calendar可以用于排除某些不需要触发Job执行的时间段。如：可以创建一个每周工作日早上9:30的Trigger，然后再加一个Calendar对象，将公休日排除掉。

Calendar可以是任意一个实现了Calendar接口的可被序列化的对象：

```java
package org.quartz;

public interface Calendar {
    public boolean isTimeIncluded(long timeStamp);
    public long getNextIncludedTime(long timeStamp);
}
```

注意到上面两个方法的参数都是毫秒时间戳，也就是可以进行毫秒级别的过滤。但大部分情况下，都是要排除掉一整天，因此为了方便操作，Quartz提供了一个`org.quartz.impl.HolidayCalendar`类。

Calendar必须实例化，并通过`addCalendar(..)`方法注册到调度器上。如果使用`HolidayCalendar`，在实例化之后，需要调用其`addExcludedDate(Date date)`方法将想要排除的日期排除掉。

同一个Calendar实例可以被多个Trigger使用。

```java
HolidayCalendar cal = new HolidayCalendar();
cal.addExcludedDate( someDate );
cal.addExcludedDate( someOtherDate );

sched.addCalendar("myHolidays", cal, false);

Trigger t = newTrigger()
    .withIdentity("myTrigger")
    .forJob("myJob")
    .withSchedule(dailyAtHourAndMinute(9, 30)) // execute job daily at 9:30
    .modifiedByCalendar("myHolidays") // but not on holidays
    .build();

// .. schedule job with trigger

Trigger t2 = newTrigger()
    .withIdentity("myTrigger2")
    .forJob("myJob2")
    .withSchedule(dailyAtHourAndMinute(11, 30)) // execute job daily at 11:30
    .modifiedByCalendar("myHolidays") // but not on holidays
    .build();

// .. schedule job with trigger2
```

Quartz的`org.quartz.impl.calendar`包下提供了一些内置的Calendar实现可以直接使用。

## Simple Trigger

SimpleTrigger用于在特定时刻执行一次或特定时刻开始以固定的时间间隔重复执行的场景。如：在2018-07-03 16:33:20执行一次，或者在这之后，每10秒执行一次，重复5次。

因此SimpleTrigger包含startTime、endTime、repeatCount、repeatInterval这几个属性，其他属性都好理解，只有endTime稍有不同。

* startTime开始时间
* repeatCount可以为0或一个正integer或静态常量`SimpleTrigger.REPEAT_INDEFINITELY`。
* repeatInterval必须为0或一个正long，单位是毫秒。如果repeatInterval设为0，则repeatCount指定数量的Trigger会被同时触发，或者说接近同时的被调度器触发。
* endTime如果指定了值，会**覆盖**repeatCount属性的值。这样可以用于创建一个类似于<每10秒钟运行一次直到某一特定时刻结束>的Trigger。此时也无需计算startTime和endTime之间的repeatCount，只需要指定endTime，repeatCount=REPEAT_INDEFINITELY(或者指定一个比实际重复的次数大的数)即可。

这里就会用到Quartz提供的DataBuilder类，它可以很方便的基于开始时间或结束时间来计算并创建触发时间。

SimpleTrigger实例可以使用TriggerBuilder和SimpleScheduleBuilder来创建：

```java
import static org.quartz.TriggerBuilder.*;
import static org.quartz.SimpleScheduleBuilder.*;
import static org.quartz.DateBuilder.*;
```

如下，是一些示例：

1. 特定时间触发，不重复执行
   ```java
   SimpleTrigger trigger = (SimpleTrigger) newTrigger()
       .withIdentity("trigger1", "group1")
       .startAt(myStartTime) // some Date
       .forJob("job1", "group1") // identify job with name, group strings
       .build();
   ```
2. 特定时间触发，每10秒执行一次，重复10次
   ```java
   trigger = newTrigger()
       .withIdentity("trigger3", "group1")
       .startAt(myTimeToStartFiring)  // if a start time is not given (if this line were omitted), "now" is implied
       .withSchedule(simpleSchedule()
           .withIntervalInSeconds(10)
           .withRepeatCount(10)) // note that 10 repeats will give a total of 11 firings
       .forJob(myJob) // identify job with handle to its JobDetail itself                   
       .build();
   ```
3. 从现在开始5分钟后执行一次
   ```java
   trigger = (SimpleTrigger) newTrigger()
       .withIdentity("trigger5", "group1")
       .startAt(futureDate(5, IntervalUnit.MINUTE)) // use DateBuilder to create a date in the future
       .forJob(myJobKey) // identify job with its JobKey
       .build();
   ```
4. 从现在开始执行，每5分钟执行一次，直到22:00
   ```java
   trigger = newTrigger()
       .withIdentity("trigger7", "group1")
       .withSchedule(simpleSchedule()
           .withIntervalInMinutes(5)
           .repeatForever())
       .endAt(dateOf(22, 0, 0))
       .build();
   ```
5. 从下一个偶数整点开始，每2小时运行一次，不停止
   ```java
   trigger = newTrigger()
       .withIdentity("trigger8") // because group is not specified, "trigger8" will be in the default group
       .startAt(evenHourDate(null)) // get the next even-hour (minutes and seconds zero ("00:00"))
       .withSchedule(simpleSchedule()
           .withIntervalInHours(2)
           .repeatForever())
       // note that in this example, 'forJob(..)' is not called
       //  - which is valid if the trigger is passed to the scheduler along with the job  
       .build();

   scheduler.scheduleJob(trigger, job);
   ```

可以参考Quartz Javadoc，熟悉一下`TriggerBuilder`和`SimpleScheduleBuilder`类提供的方法。**Quartz提供的Builder类，如果没有明确的调用某个方法设置属性值，通常都会默认设置一个合理的值。**

SimpleTrigger的"misfire instruction"有很多，都作为常量定义在了SimpleTrigger类中。

```
MISFIRE_INSTRUCTION_IGNORE_MISFIRE_POLICY
MISFIRE_INSTRUCTION_FIRE_NOW
MISFIRE_INSTRUCTION_RESCHEDULE_NOW_WITH_EXISTING_REPEAT_COUNT
MISFIRE_INSTRUCTION_RESCHEDULE_NOW_WITH_REMAINING_REPEAT_COUNT
MISFIRE_INSTRUCTION_RESCHEDULE_NEXT_WITH_REMAINING_COUNT
MISFIRE_INSTRUCTION_RESCHEDULE_NEXT_WITH_EXISTING_COUNT
```

上文提到过，所有Trigger默认"misfire instruction"都是`Trigger.MISFIRE_INSTRUCTION_SMART_POLICY`。

如果"smart policy"用于SimpleTrigger，它会基于Trigger实例的配置和状态动态的在已有的instruction中选择一个。`SimpleTrigger.updateAfterMisfire()`方法的代码说明了动态选择的过程。

创建SimpleTrigger时，可以在simpleSchedule()部分指定"misfire instruction"：

```java
trigger = newTrigger()
    .withIdentity("trigger7", "group1")
    .withSchedule(simpleSchedule()
        .withIntervalInMinutes(5)
        .repeatForever()
        .withMisfireHandlingInstructionNextWithExistingCount())
    .build();
```

## CronTrigger

CronTrigger用于基于日历时间的任务调度的场景。如：每周5下午，或每个工作日上午9:30，或一月份的每周一三五的上午9:00到10:00之间每5分钟一次等。

CronTrigger也有startTime属性指定何时开始，endTime(可选)属性指定何时停止。

### Cron表达式

Cron表达式是用来配置CronTrigger实例的，它实际上是一个由7个子表达式组成的字符串，描述了调度时间的详情信息。7个子表达式由空格分开，分别代表：

* 秒
* 分
* 时
* 日期
* 月份
* 星期
* 年(可选)

例如：表达式`0 0 12 ? * WED`表示每周三的12:00

1. 单个子表达式，可以包含范围或(和)列表描述。如：星期这个字段，可以写成`MON-FRI`、`MON,WED,FRI`或`MON-WED,SAT`。
2. 通配符指该字段的所有值都满足条件，因此上例中月份字段的`*`号就表示每个月，而星期字段的`*`就表示周内的每一天。
3. 每个字段都有取值范围，`秒`和`分`字段取值0-59，`时`字段取值0-23，`日期`字段取值1-31(要考虑到不是每个月都有31号)，`月份`字段取值0-11或JAN/FEB/MAR/APR/MAY/JUN/JUL/AUG/SEP/OCT/NOV/DEC，`星期`字段取值1-7(1=周日)或SUN/MON/TUE/WED/THU/FRI/SAT。
4. `/`表达每X单位一次。如：在`分`字段，`0/15`表示从第0分开始每15分钟执行一次，`3/20`表示从第3分钟开始每20分钟执行一次(相当于配置为`3,23,43`)。**注意**：`/35`不是指每35分钟执行一次，而是从第0分钟开始每35分钟执行一次，相当于配置为`0,35`。
5. `?`只允许在`日期`和`星期`字段使用，意思是不指定值，主要用于只需要指定这两个字段中的一个而不指定另一个的场合。
6. `L`只允许在`日期`和`星期`字段使用，意思是最后一天，它在这两个字段的含义略有不同。`月份`字段`L`表示每个月的最后一天(1月31号/平年2月28号/闰年2月29号等)。`星期`字段`L`表示7或SAT，但是如果是`星期+L`，则表示每个月的最后一个周几的意思，如：`星期`字段的`6L`或`FRIL`表示每个月的最后一个周五。`L`也可以添加偏移量，如`L-3`表示距离最后一天还有3天的时刻。**注意**：如果使用了`L`，就不要再指定列表或范围值了。
7. `W`指的是最接近指定日期的工作日，如：`日期`字段`15W`表示距离每月15号最近的工作日。
8. `#`指的是每月第N个周几，如：`星期`字段`6#3`或`FRI#3`表示每个月的第3个周五。

如下，是一个示例(也可以参考`org.quartz.CronExpression`的Javadoc获取更多示例)：

1. `0 0/5 * * * ?`：每5分钟执行一次
2. `10 0/5 * * * ?`：每5分钟的第10秒的时候执行一次，如：10:00:10, 10:05:10 ...
3. `0 30 10-13 ? * WED,FRI`：每周三和周五的10:30/11:30/12:30/13:30执行
4. `0 0/30 8-9 5,20 * ?`：每月5号和20号的8:00/8:30/9:00/9:30执行

**注意**：对于比较复杂的场景，如：每天9-10点间每5钟执行一次，同时13-22点间每20分钟执行一次，建议配置两个Trigger，然后注册到同一个Job实例上。

### Building CronTriggers

CronTrigger实例可以使用TriggerBuilder和CronScheduleBuilder来创建：

```java
import static org.quartz.TriggerBuilder.*;
import static org.quartz.CronScheduleBuilder.*;
import static org.quartz.DateBuilder.*;
```

如下，是一些示例：

1. 每天的8-17点每隔一分钟执行一次
   ```java
   trigger = newTrigger()
       .withIdentity("trigger3", "group1")
       .withSchedule(cronSchedule("0 0/2 8-17 * * ?"))
       .forJob("myJob", "group1")
       .build();
   ```

2. 每天的10:42执行一次
   ```java
   trigger = newTrigger()
       .withIdentity("trigger3", "group1")
       .withSchedule(dailyAtHourAndMinute(10, 42))
       .forJob(myJobKey)
       .build();
   ```
   或
   ```java
   trigger = newTrigger()
       .withIdentity("trigger3", "group1")
       .withSchedule(cronSchedule("0 42 10 * * ?"))
       .forJob(myJobKey)
       .build();
   ```

3. 在指定时区的每周三的10:42执行一次
   ```java
   trigger = newTrigger()
       .withIdentity("trigger3", "group1")
       .withSchedule(weeklyOnDayAndHourAndMinute(DateBuilder.WEDNESDAY, 10, 42))
       .forJob(myJobKey)
       .inTimeZone(TimeZone.getTimeZone("America/Los_Angeles"))
       .build();
   ```
   或
   ```java
   trigger = newTrigger()
       .withIdentity("trigger3", "group1")
       .withSchedule(cronSchedule("0 42 10 ? * WED"))
       .inTimeZone(TimeZone.getTimeZone("America/Los_Angeles"))
       .forJob(myJobKey)
       .build();
   ```

参考[Cron Maker](http://www.cronmaker.com/ "A utility helps you to build cron expressions")可以快速创建Cron表达式。

### CronTrigger Misfire Instructions

CronTrigger的"misfire instruction"都作为常量定义在了CronTrigger类中。

```
MISFIRE_INSTRUCTION_IGNORE_MISFIRE_POLICY
MISFIRE_INSTRUCTION_DO_NOTHING
MISFIRE_INSTRUCTION_FIRE_NOW
```

所有Trigger默认"misfire instruction"都是`Trigger.MISFIRE_INSTRUCTION_SMART_POLICY`。如果"smart policy"用于CronTrigger，它会选择`MISFIRE_INSTRUCTION_FIRE_NOW`。`CronTrigger.updateAfterMisfire()`方法的代码说明了选择的过程。

创建CronTrigger时，可以在cronSchedule()部分指定"misfire instruction"：

```java
trigger = newTrigger()
    .withIdentity("trigger3", "group1")
    .withSchedule(cronSchedule("0 0/2 8-17 * * ?")
        .withMisfireHandlingInstructionFireAndProceed())
    .forJob("myJob", "group1")
    .build();
```

## TriggerListener and JobListener

Listener对象用于调度过程中某一事件发生后执行用户自定义的动作。

Trigger相关的事件有：
* Trigger firings(Trigger被触发)
* Trigger mis-firings(Trigger由于调度器关闭或线程数不足错过了触发时机)
* Trigger completions(Trigger触发的Job执行结束了)

```java
public interface TriggerListener {
    public String getName();
    public void triggerFired(Trigger trigger, JobExecutionContext context);
    public boolean vetoJobExecution(Trigger trigger, JobExecutionContext context);
    public void triggerMisfired(Trigger trigger);
    public void triggerComplete(Trigger trigger, JobExecutionContext context, int triggerInstructionCode);
}
```

Job相关的事件有：
* Job即将被执行
* Job执行结束

```java
public interface JobListener {
    public String getName();
    public void jobToBeExecuted(JobExecutionContext context);
    public void jobExecutionVetoed(JobExecutionContext context);
    public void jobWasExecuted(JobExecutionContext context, JobExecutionException jobException);
}
```

创建Listener只需要实现`org.quartz.TriggerListener`或`org.quartz.JobListener`接口，然后注册到调度器即可。每个Listener必须有一个name，可以通过`getName()`方法返回这个name。

在通过调度器的ListenerManager注册Listener时，还需要添加一个Matcher，用来描述要监听哪些Job/Trigger。

**注意**：Listener是在运行时期间注册到调度器的，它不会跟随Job和Trigger存储在JobStore中。因为Listener通常是与应用程序集成的，所以每次程序运行时都需要重新注册Listener。

如下，是一些示例：

import语句：

```java
import static org.quartz.JobKey.*;
import static org.quartz.impl.matchers.KeyMatcher.*;
import static org.quartz.impl.matchers.GroupMatcher.*;
import static org.quartz.impl.matchers.AndMatcher.*;
import static org.quartz.impl.matchers.OrMatcher.*;
import static org.quartz.impl.matchers.EverythingMatcher.*;
```

1. 给指定的Job添加一个JobListener
   ```java
   scheduler.getListenerManager()
       .addJobListener(
           myJobListener,
           KeyMatcher.jobKeyEquals(
               new JobKey("myJobName", "myJobGroup")
           )
       );
   ```
   或
   ```java
   scheduler.getListenerManager()
       .addJobListener(
           myJobListener,
           jobKeyEquals(
               jobKey("myJobName", "myJobGroup")
           )
       );
   ```

2. 给指定Group下的所有Job添加一个Listener
   ```java
   scheduler.getListenerManager()
       .addJobListener(
           myJobListener,
           jobGroupEquals("myJobGroup")
       );
   ```

3. 给指定的两个Group下的所有Job添加一个Listener
   ```java
   scheduler.getListenerManager()
       .addJobListener(
           myJobListener,
           or(
               jobGroupEquals("myJobGroup"),
               jobGroupEquals("yourGroup")
           )
       );
   ```

4. 给所有Job添加一个Listener
   ```java
   scheduler.getListenerManager()
       .addJobListener(
           myJobListener,
           allJobs()
       );
   ```

TriggerListener添加方式类似JobListener。

Listener并不是很常用，但当程序需要创建事件通知时，Job本身不用显式的通知应用程序，还是很方便的。

## SchedulerListener

SchedulerListener与TriggerListener/JobListener类似，接收Scheduler自己的事件通知。

Scheduler相关的事件有：
* 添加Job/Trigger
* 移除Job/Trigger
* scheduler发生严重错误
* scheduler关闭等

```java
public interface SchedulerListener {
    public void jobScheduled(Trigger trigger);
    public void jobUnscheduled(String triggerName, String triggerGroup);
    public void triggerFinalized(Trigger trigger);
    public void triggersPaused(String triggerName, String triggerGroup);
    public void triggersResumed(String triggerName, String triggerGroup);
    public void jobsPaused(String jobName, String jobGroup);
    public void jobsResumed(String jobName, String jobGroup);
    public void schedulerError(String msg, SchedulerException cause);
    public void schedulerStarted();
    public void schedulerInStandbyMode();
    public void schedulerShutdown();
    public void schedulingDataCleared();
}
```

SchedulerListener可以是任何实现了`org.quartz.SchedulerListener`接口的类的对象，通过scheduler的ListenerManager注册。

1. 添加SchedulerListener
   ```java
   scheduler.getListenerManager().addSchedulerListener(mySchedListener);
   ```

2. 移除SchedulerListener
   ```java
   scheduler.getListenerManager().removeSchedulerListener(mySchedListener);
   ```

## Job Store

JobStore是保存并跟踪传递给Scheduler的所有数据：Job、Trigger、Calendar等。选择合适的JobStore非常重要，而且只要理解了不同JobStore的区别也很容易选择。JobStore需要在配置文件或代码里进行指定，它最终会传递给SchedulerFactory用于创建Scheduler实例。

**注意**：不要在代码里直接使用JobStore的实例，它是Quartz内部使用的一个对象，用户只需要告诉Quartz使用哪个JobStore，剩下的事件交给框架处理就好了。

### RAMJobStore

RAMJobStore是最简单，也是最高效的JobStore，它将所有数据存放在内存当中。缺点是程序关闭后，所有的调度信息都会丢失，它无法实现Job和Trigger的非易失性(non-volatility)设置。有些应用希望如此，但有些应用则不然。

配置非常简单，只需要指定JobStore的类名为RAMJobStore即可：
```properties
org.quartz.jobStore.class=org.quartz.simpl.RAMJobStore
```

### JDBCJobStore

JDBCJobStore将所有数据存储在数据库中。因此配置相对RAMJobStore复杂一些，而且也会慢一些，但性能影响不大，尤其是为数据库表创建了主键索引之后。网络状况良好的情况下，检索和更新Trigger的延时在10ms以内。

JDBCJobStore可以支持多种类型的数据库：Oracle/PostgreSQL/MySQL/MSSQL/HSQLDB/DB2。使用JDBCJobStore之前需要先创建Quartz的相关表，数据库脚本在发布包的`docs/dbTables`目录下可以找到。如果没有找到匹配的数据库脚本可以找一个类似的脚本修改一下来用。有一点需要注意，所有的表名都带有`QRTZ_`的前缀，这个前缀可以根据需要进行修改，但相应的也要修改Quartz的配置。不同的前缀可以用来在同一数据库中区分不同Scheduler实例的数据。

数据库表建好之后，还需要添加一些必要的配置来激活JDBCJobStore。需要确定应用程序采用哪种事务类型：

* 如果不需要将调度指令关联到其他事务中，就要配置为`JobStoreTX`
* 如果要将调度指令关联到应用的事务中，就要配置为`JobStoreCMT`，这时Quartz会让应用程序来管理事务

最后就是要设置一个数据源，有几种不同的方式：一种是Quartz自己创建和管理数据源，此时需要提供所有数据库连接信息。另一种是使用应用程序提供的数据源，这时只需要提供数据源的JNDI名称即可。详细的配置可以参考Quartz官方示例。

使用JDBCJobStore(假设使用的是StdSchedulerFactory)首先要选择一个JobStore类：

* `org.quartz.impl.jdbcjobstore.JobStoreTX`
* `org.quartz.impl.jdbcjobstore.JobStoreCMT`

然后选择一个DriverDelegate，不同的数据库配置可能不同，这些实现类都在`org.quartz.impl.jdbcjobstore`包里可以找到。

* `StdJDBCDelegate`是一个通用的配置，Quartz仅为使用`StdJDBCDelegate`有问题的数据库创建了专用的delegate。
* `DB2v6Delegate`适用于DB2 V6以及之前的版本
* `HSQLDBDelegate`适用于HSQLDB
* `MSSQLDelegate`适用于Microsoft SQLServer
* `PostgreSQLDelegate`适用于PostgreSQL
* `WeblogicDelegate`适用于WebLogic的JDBC Driver
* `OracleDelegate`适用于Oracle

最后就是配置表名前缀和数据源。如下，是一些示例：

```properties
#============================================================================
# Configure JobStore  
#============================================================================

org.quartz.jobStore.class=org.quartz.impl.jdbcjobstore.JobStoreTX
org.quartz.jobStore.driverDelegateClass=org.quartz.impl.jdbcjobstore.PostgreSQLDelegate
org.quartz.jobStore.useProperties=false
org.quartz.jobStore.dataSource=myDS
org.quartz.jobStore.tablePrefix=QRTZ_
org.quartz.jobStore.isClustered=false

#============================================================================
# Configure Datasources  
#============================================================================

org.quartz.dataSource.myDS.driver=org.postgresql.Driver
org.quartz.dataSource.myDS.URL=jdbc:postgresql://localhost/dev
org.quartz.dataSource.myDS.user=postgre
org.quartz.dataSource.myDS.password=postgre
org.quartz.dataSource.myDS.maxConnections=5
```

**注意**：
* 如果调度器很繁忙(几乎每时每刻都有与线程池内线程数相等的Job在运行)，那需要将数据源的连接数配置为：`线程数+2`。
* 如果将`org.quartz.jobStore.useProperties`配置设为`true`，则JDBCJobStore会将JobDataMap里所有value当作String，这样就能将这些数据存储为键-值对，而无需序列为二进制数据存储为BLOB类型。从长远来看，这样更安全。

### TerracottaJobStore

TerracottaJobStore无需数据库即可提供扩展性和健壮性，支持集群模式或非集群模式下运行，为Job数据提供存储介质，性能比JDBCJobStore要好，但比RAMJobStore慢一些。

使用如下配置启用TerracottaJobStore：

```properties
# JobStore类名
org.quartz.jobStore.class=org.terracotta.quartz.TerracottaJobStore
# Terracotta Server的地址
org.quartz.jobStore.tcConfigUrl=localhost:9510
```

## Configuration

Quartz是模块化的，需要将这些模块整合起来才能工作。主要的几个模块有：
* ThreadPool 线程池
* JobStore Job数据存储方式
* DataSource 根据需要进行配置
* Scheduler 调度器本身

ThreadPool线程池提供了用于Quartz执行Job的线程，线程数越多，同时运行的Job越多，但过多的线程可能会拖慢系统。
* 任意特定时间的Job数量都不到100个，通常不是同时运行，而且执行很快的情况下，5个线程就足够了。
* 当有数万个不同时间表的Trigger，任意给定时刻平均有10-100个Job在运行时，就需要10/15/50甚至100个线程了。

线程数的确定完全依赖于调度器程序的内容，没有准则，但保持线程数尽量小(但要保证有足够的线程用于Job正常运行)。

**注意**：如果Trigger的触发时间到了，但没有空闲线程可用时，Quartz会block(pause)直到有空闲的线程可用，Job才会执行，这会比Job本来应该执行的时刻晚几毫秒。如果在Scheduler配置的"misfire threshold"时间内仍然没有可用线程，则会导致Trigger misfire。

ThreadPool接口定义在`org.quartz.spi`包下，用户可以编写自己的ThreadPool实现。Quartz默认提供了一个简单的线程池`org.quartz.simpl.SimpleThreadPool`，是固定线程数的，不会增长也不会减少，但经过了全面测试且十分健壮，几乎所有用户都会使用这个线程池。

JobStore和DataSource上文已经提到过了，所有的JobStore都实现了`org.quartz.spi.JobStore`接口，如果Quartz提供的JobStore不能满足要求用户也可以编写自己的JobStore实现。

最后就是创建一个Scheduler实例，需要为Scheduler指定一个名字，设置RMI参数，并传递JobStore和ThreadPool实例。RMI参数包括是否启动RMI服务(使用Scheduler自身用于远程连接)及要使用的主机和端口等。StdSchedulerFactory (discussed below) can also produce Scheduler instances that are actually proxies (RMI stubs) to Schedulers created in remote processes.

### StdSchedulerFactory

StdSchedulerFactory是`org.quartz.SchedulerFactory`接口的实现类。它使用一组属性配置(`java.util.Properties`)去创建和初始化Scheduler。这些属性可以从配置文件加载也可以在代码中直接传递。简单的调用`getScheduler()`方法即可创建、初始化(ThreadPool/JobStore/DataSource)并返回Scheduler实例。

Quartz的详细配置可参考[这里](http://www.quartz-scheduler.org/documentation/quartz-2.2.x/configuration/ "Quartz Configuration Reference")，也可以在发布包里找到一些配置示例。

### DirectSchedulerFactory

DirectSchedulerFactory是另一个SchedulerFactory实现，它用于以编码方式创建Scheduler实例的场合。不建议使用的原因有两点：
* 用户必须更好的理解自己正在做什么
* 不允许声明式的配置，必须硬编码所有的设置

### Logging

Quartz使用SLF4J框架记录日志，如果要获取Trigger触发和Job执行相关的额外信息，可以启用这两个插件：
* `org.quartz.plugins.history.LoggingJobHistoryPlugin`
* `org.quartz.plugins.history.LoggingTriggerHistoryPlugin`

