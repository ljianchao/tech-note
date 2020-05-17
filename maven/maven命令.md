# maven命令


建一个 JAVA 项目： mvn archetype:create -DgroupId=com.demo -DartifactId=App
建一个 web 项目 ： mvn archetype:create -DgroupId=com.demo -DartifactId=web-app -DarchetypeArtifactId=maven-archetype-webapp

mvn archetype:create -DgroupId=cn.damai.demo -DartifactId=demo
mvn archetype:create -DgroupId=cn.damai.demo -DartifactId=demo.modules
mvn archetype:create -DgroupId=cn.damai.demo -DartifactId=demo.apps

mvn archetype:create -DgroupId=cn.damai.demo -DartifactId=cn.damai.demo.api	//<packaging>jar</packaging>
mvn archetype:create -DgroupId=cn.damai.demo -DartifactId=cn.damai.demo.impl 	//<packaging>jar</packaging>

mvn archetype:create -DgroupId=cn.damai.demo -DartifactId=demo.web -DarchetypeArtifactId=maven-archetype-webapp


使用maven建立多模块web项目有时候我们需要创建多模块maven项目，即将传统的ssh项目中的model，dao，manager，service，view层划分成单个的maven module。具体步骤如下:

## 创建maven工程
#### 生成父级工程
	命令：mvn archetype:generate -DgroupId=com.mljr.middleware.demo.sleuth -DartifactId=spring-sleuth-demo
	进入创建的目录spring-sleuth-demo，删除src文件夹，打开pom.xml，将packaging属性修改为pom（表示为父工程）

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=common-log-parent

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=m-grayscale 

#### 生成子工程：
	在spring-sleuth-demo目录中执行以下命令：  
	mvn archetype:generate -DgroupId=com.mljr.middleware.demo.sleuth -DartifactId=begin-service -Dpackage=com.mljr.middleware.demo.sleuth.begin

	mvn archetype:generate -DgroupId=com.mljr.middleware.demo.sleuth -DartifactId=continue-service -Dpackage=com.mljr.middleware.demo.sleuth.continue

	mvn archetype:generate -DgroupId=com.mljr.middleware.demo.sleuth -DartifactId=end-service -Dpackage=com.mljr.middleware.demo.sleuth.end
  
	mvn archetype:generate -DgroupId=com.mljr.middleware.demo.sleuth -DartifactId=domain -Dpackage=com.mljr.middleware.demo.sleuth.domain -DarchetypeArtifactId=maven-archetype-quickstart

	mvn archetype:generate -DgroupId=com.mljr.kwrap -DartifactId=kwrap-sleuth -Dpackage=com.mljr.kwrap.sleuth
 -DarchetypeArtifactId=maven-archetype-quickstart

	mvn archetype:generate -DgroupId=com.mljr.kwrap -DartifactId=kwrap-spring-boot-autoconfigure -Dpackage=com.mljr.kwrap.autoconfigure -DarchetypeArtifactId=maven-archetype-quickstart

	mvn archetype:generate -DgroupId=com.mljr.kwrap -DartifactId=kwrap-log-trace -Dpackage=com.mljr.kwrap.log.trace -DarchetypeArtifactId=maven-archetype-quickstart

	mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=mdiamond-spring-boot-autoconfigure -Dpackage=com.mljr.fuxi.xdiamond.autoconfigure -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=common-log -Dpackage=com.mljr.msf.log -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=common-log-sleuth-core -Dpackage=com.mljr.msf.log.sleuth.core -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=common-log-spring-boot-autoconfigure -Dpackage=com.mljr.msf.log.autoconfigure -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=common-log-spring-boot-starter -Dpackage=com.mljr.msf.log.starter -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=m-grayscale-core -Dpackage=com.mljr.msf.mgrayscale.core -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=m-grayscale-dependencies -Dpackage=com.mljr.msf.mgrayscale.dependencies -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=m-grayscale-spring-boot-autoconfigure -Dpackage=com.mljr.msf.mgrayscale.autoconfigure -DarchetypeArtifactId=maven-archetype-quickstart

mvn archetype:generate -DgroupId=com.mljr.msf -DartifactId=m-grayscale-spring-boot-starter -Dpackage=com.mljr.msf.mgrayscale.starter -DarchetypeArtifactId=maven-archetype-quickstart

创建business-tracing
- 创建父目录
mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-parent

- 创建子目录
mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-reporter -Dpackage=com.mljr.msf.tracing.reporter

mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-core -Dpackage=com.mljr.msf.tracing.core

mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-bom -Dpackage=com.mljr.msf.tracing.bom

mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-instrumentation -Dpackage=com.mljr.msf.tracing.instrumentation

mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-instrumentation-mysql5 -Dpackage=com.mljr.msf.tracing.instrumentation.mysql5

mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-instrumentation-druid -Dpackage=com.mljr.msf.tracing.instrumentation.druid

mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-instrumentation-spring-web -Dpackage=com.mljr.msf.tracing.instrumentation.spring.web

mvn archetype:generate -DgroupId=com.mljr.msf.tracing -DartifactId=business-tracing-instrumentation-spring-webmvc -Dpackage=com.mljr.msf.tracing.instrumentation.spring.webmvc


#### 特例（web子项目）
	命令
	mvn archetype:generate -DgroupId=com.duanqu -DartifactId=duanqu-web -Dpackage=com.duanqu.web -DarchetypeArtifactId=maven-archetype-webapp

	其中 -DarchetypeArtifactId=maven-archetype-webapp表示生成的子项目为web工程

mvn archetype:generate -DgroupId=jc.demo -DartifactId=spring-web-demo -Dpackage=cn.demo.spring.web -DarchetypeArtifactId=maven-archetype-webapp

mvn archetype:create ：创建 Maven 项目 
mvn compile ：编译源代码
mvn test-compile ：编译测试代码
mvn test ： 运行应用程序中的单元测试
mvn site ： 生成项目相关信息的网站
mvn clean ：清除目标目录中的生成结果
mvn package ： 依据项目生成 jar 文件
mvn install ：在本地 Repository 中安装 jar
mvn eclipse:eclipse ：生成 Eclipse 项目文件
mvn -Dmaven.test.skip=true : 忽略测试文档编译
Mvn eclipse:eclipse:导入文件到eclips中

# maven依赖
mvn help:effective-pom：查看有效的pom配置，需定位到pom.xml文件所在的位置运行命令
mvn dependency:resolve 查看依赖
mvn dependency:analyze 查看依赖错误
mvn dependency:tree 查看依赖树
mvn -X compile dependency:tree -Dverbose >a.log
开发在本地执行此命令可以检查Jar包版本冲突：mvn -X compile dependency:tree -Dverbose >a.log，然后在a.log文件里查找 是否有"omitted for conflict with"，如果有，则表示jar包有版本冲突 ；

mvn dependency:tree -Dverbose -Dincludes=commons-collections


mvn clean install -Pproduction -X ：profile配置的打包

创建maven java项目:
mvn archetype:generate -DgroupId=cn.damai -DartifactId=ceph-buildtest -DarchetypeArtifactId=maven-archetype-quickstart

创建maven web项目:
mvn archetype:generate -DgroupId=cn.damai -DartifactId=ceph-buildtest -DarchetypeArtifactId=maven-archetype-webapp

在项目基础目录下运行项目：
mvn exec:java -Dexec.mainClass=org.sonatype.mavenbook.weather.Main

构建带有依赖的jar包
mvn assembly:assembly

## 打包
mvn clean -P test source:jar install deploy -Dmaven.test.skip

mvn clean -P test source:jar install -Dmaven.test.skip

mvn clean -P dev source:jar install -Dmaven.test.skip

mvn clean -P dev source:jar install deploy -Dmaven.test.skip

-U参数强制检查快照版本的更新
mvn clean -U -P dev source:jar install -Dmaven.test.skip=true

## findbugs测试
mvn clean package site -Dmaven.test.skip

