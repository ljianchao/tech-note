# pom文件

## bom定义

```xml
<?xml version="1.0" encoding="UTF-8"?>

<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.base</groupId>
    <artifactId>base-common-bom</artifactId>
    <version>1.0.0-SNAPSHOT</version>
    <packaging>pom</packaging>
    <name>base-common-bom</name>
    <description>Base parent pom, managing plugins and dependencies for demo projects.</description>

    <properties>
        <java.version>1.8</java.version>
        <maven.compiler.source>${java.version}</maven.compiler.source>
        <maven.compiler.target>${java.version}</maven.compiler.target>
        <maven.build.timestamp.format>yyyy-MM-dd HH:mm:ss</maven.build.timestamp.format>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
        <main.basedir>${basedir}</main.basedir>
        <profiles.dir>src/profiles</profiles.dir>

        <!-- Spring -->
        <spring-framework.version>5.2.7.RELEASE</spring-framework.version>
        <spring-boot.version>2.3.1.RELEASE</spring-boot.version>
        <spring-cloud.version>Hoxton.SR5</spring-cloud.version>

        <!-- jdbc -->
        <mysql-connector-java.version>8.0.20</mysql-connector-java.version>
        <mybatis.version>3.5.4</mybatis.version>
        <mybatis-spring.version>2.0.4</mybatis-spring.version>
        <mybatis-spring-boot-starter.verison>2.1.2</mybatis-spring-boot-starter.verison>
        <tk.mybatis.version>4.1.5</tk.mybatis.version>
        <pagehelper.version>5.1.11</pagehelper.version>
        <pagehelper-spring-boot-starter.version>1.2.13</pagehelper-spring-boot-starter.version>
        <HikariCP-java6.version>2.3.13</HikariCP-java6.version>
        <HikariCP.version>3.4.5</HikariCP.version>
        <druid.version>1.1.22</druid.version>

        <!-- Commons -->
        <commons-lang.version>2.6</commons-lang.version>
        <commons-lang3.version>3.10</commons-lang3.version>
        <commons-validator.version>1.6</commons-validator.version>
        <commons-collections.version>3.2.2</commons-collections.version>
        <commons-collections4.version>4.4</commons-collections4.version>
        <commons-beanutils.version>1.9.4</commons-beanutils.version>
        <commons-codec.version>1.14</commons-codec.version>
        <commons-logging.version>1.2</commons-logging.version>
        <commons-io.version>2.6</commons-io.version>
        <commons-fileupload.version>1.4</commons-fileupload.version>
        <commons-csv.version>1.8</commons-csv.version>

        <!-- Http -->
        <apache-httpclient.version>4.5.12</apache-httpclient.version>
        <apache-httpcore.version>4.4.13</apache-httpcore.version>

        <!-- Aliyun -->
        <aliyun-java-sdk-core.version>4.5.1</aliyun-java-sdk-core.version>
        <aliyun-java-sdk-dysmsapi.version>2.1.0</aliyun-java-sdk-dysmsapi.version>

        <!-- Dubbo -->
        <dubbo.version>2.7.7</dubbo.version>
        <dubbo-spring-boot-starter>2.7.7</dubbo-spring-boot-starter>
        <zookeeper.version>3.6.1</zookeeper.version>
        <zkclient.version>0.11</zkclient.version>

        <!-- log -->
        <slf4j.version>1.7.30</slf4j.version>
        <log4j2.version>2.13.3</log4j2.version>
        <disruptor.version>3.4.2</disruptor.version>

        <!-- Others -->
        <fastjson.version>1.2.68</fastjson.version>
        <gson.version>2.8.6</gson.version>
        <netty.version>4.1.50.Final</netty.version>
        <lombok.version>1.18.12</lombok.version>

        <!-- Plugins-->
        <maven-clean-plugin.version>3.1.0</maven-clean-plugin.version>
        <maven-resources-plugin.version>3.1.0</maven-resources-plugin.version>
        <maven-compiler-plugin.version>3.8.1</maven-compiler-plugin.version>
        <maven-javadoc-plugin.version>3.2.0</maven-javadoc-plugin.version>
        <maven-source-plugin.version>3.2.1</maven-source-plugin.version>
        <maven-failsafe-plugin.version>3.0.0-M4</maven-failsafe-plugin.version>
        <maven-surefire-plugin.version>3.0.0-M4</maven-surefire-plugin.version>
        <maven-jar-plugin.version>3.2.0</maven-jar-plugin.version>
        <maven-war-plugin.version>3.2.3</maven-war-plugin.version>
        <maven-assembly-plugin.version>3.3.0</maven-assembly-plugin.version>
        <maven-dependency-plugin.version>3.1.2</maven-dependency-plugin.version>
        <maven-checkstyle-plugin.version>3.1.1</maven-checkstyle-plugin.version>
        <maven-install-plugin.version>3.0.0-M1</maven-install-plugin.version>
        <maven-deploy-plugin.version>3.0.0-M1</maven-deploy-plugin.version>
        <lombok-maven-plugin.version>1.18.12.0</lombok-maven-plugin.version>
    </properties>

    <dependencyManagement>
        <dependencies>
            <!-- Spring -->
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-framework-bom</artifactId>
                <version>${spring-framework.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
            <dependency>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-dependencies</artifactId>
                <version>${spring-boot.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
            <dependency>
                <groupId>org.springframework.cloud</groupId>
                <artifactId>spring-cloud-dependencies</artifactId>
                <version>${spring-cloud.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>

            <!--jdbc -->
            <dependency>
                <groupId>mysql</groupId>
                <artifactId>mysql-connector-java</artifactId>
                <version>${mysql-connector-java.version}</version>
            </dependency>
            <dependency>
                <groupId>org.mybatis</groupId>
                <artifactId>mybatis</artifactId>
                <version>${mybatis.version}</version>
            </dependency>
            <dependency>
                <groupId>org.mybatis</groupId>
                <artifactId>mybatis-spring</artifactId>
                <version>${mybatis-spring.version}</version>
            </dependency>
            <dependency>
                <groupId>org.mybatis.spring.boot</groupId>
                <artifactId>mybatis-spring-boot-starter</artifactId>
                <version>${mybatis-spring-boot-starter.verison}</version>
                <exclusions>
                    <exclusion>
                        <groupId>org.apache.tomcat</groupId>
                        <artifactId>tomcat-jdbc</artifactId>
                    </exclusion>
                </exclusions>
            </dependency>
            <dependency>
                <groupId>tk.mybatis</groupId>
                <artifactId>mapper</artifactId>
                <version>${tk.mybatis.version}</version>
            </dependency>
            <dependency>
                <groupId>com.github.pagehelper</groupId>
                <artifactId>pagehelper</artifactId>
                <version>${pagehelper.version}</version>
            </dependency>
            <dependency>
                <groupId>com.github.pagehelper</groupId>
                <artifactId>pagehelper-spring-boot-starter</artifactId>
                <version>${pagehelper-spring-boot-starter.version}</version>
            </dependency>
            <dependency>
                <groupId>com.zaxxer</groupId>
                <artifactId>HikariCP-java6</artifactId>
                <version>${HikariCP-java6.version}</version>
            </dependency>
            <dependency>
                <groupId>com.zaxxer</groupId>
                <artifactId>HikariCP</artifactId>
                <version>${HikariCP.version}</version>
            </dependency>
            <dependency>
                <groupId>com.alibaba</groupId>
                <artifactId>druid</artifactId>
                <version>${druid.version}</version>
            </dependency>

            <!--Commons -->
            <dependency>
                <groupId>commons-lang</groupId>
                <artifactId>commons-lang</artifactId>
                <version>${commons-lang.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.commons</groupId>
                <artifactId>commons-lang3</artifactId>
                <version>${commons-lang3.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-validator</groupId>
                <artifactId>commons-validator</artifactId>
                <version>${commons-validator.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-collections</groupId>
                <artifactId>commons-collections</artifactId>
                <version>${commons-collections.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.commons</groupId>
                <artifactId>commons-collections4</artifactId>
                <version>${commons-collections4.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-beanutils</groupId>
                <artifactId>commons-beanutils</artifactId>
                <version>${commons-beanutils.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-codec</groupId>
                <artifactId>commons-codec</artifactId>
                <version>${commons-codec.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-logging</groupId>
                <artifactId>commons-logging</artifactId>
                <version>${commons-logging.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-io</groupId>
                <artifactId>commons-io</artifactId>
                <version>${commons-io.version}</version>
            </dependency>
            <dependency>
                <groupId>commons-fileupload</groupId>
                <artifactId>commons-fileupload</artifactId>
                <version>${commons-fileupload.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.commons</groupId>
                <artifactId>commons-csv</artifactId>
                <version>${commons-csv.version}</version>
            </dependency>

            <!-- Http -->
            <dependency>
                <groupId>org.apache.httpcomponents</groupId>
                <artifactId>httpclient</artifactId>
                <version>${apache-httpclient.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.httpcomponents</groupId>
                <artifactId>httpcore</artifactId>
                <version>${apache-httpcore.version}</version>
            </dependency>

            <!-- Aliyun -->
            <dependency>
                <groupId>com.aliyun</groupId>
                <artifactId>aliyun-java-sdk-core</artifactId>
                <version>${aliyun-java-sdk-core.version}</version>
            </dependency>
            <dependency>
                <groupId>com.aliyun</groupId>
                <artifactId>aliyun-java-sdk-dysmsapi</artifactId>
                <version>${aliyun-java-sdk-dysmsapi.version}</version>
            </dependency>

            <!-- Dubbo -->
            <dependency>
                <groupId>org.apache.dubbo</groupId>
                <artifactId>dubbo</artifactId>
                <version>${dubbo.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.dubbo</groupId>
                <artifactId>dubbo-spring-boot-starter</artifactId>
                <version>${dubbo-spring-boot-starter}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.zookeeper</groupId>
                <artifactId>zookeeper</artifactId>
                <version>${zookeeper.version}</version>
                <exclusions>
                    <exclusion>
                        <groupId>junit</groupId>
                        <artifactId>junit</artifactId>
                    </exclusion>
                    <exclusion>
                        <artifactId>log4j</artifactId>
                        <groupId>log4j</groupId>
                    </exclusion>
                    <exclusion>
                        <groupId>org.slf4j</groupId>
                        <artifactId>slf4j-log4j12</artifactId>
                    </exclusion>
                </exclusions>
            </dependency>
            <dependency>
                <groupId>com.101tec</groupId>
                <artifactId>zkclient</artifactId>
                <version>${zkclient.version}</version>
                <exclusions>
                    <exclusion>
                        <artifactId>log4j</artifactId>
                        <groupId>log4j</groupId>
                    </exclusion>
                    <exclusion>
                        <artifactId>slf4j-log4j12</artifactId>
                        <groupId>org.slf4j</groupId>
                    </exclusion>
                </exclusions>
            </dependency>

            <!-- log -->
            <dependency>
                <groupId>org.slf4j</groupId>
                <artifactId>slf4j-api</artifactId>
                <version>${slf4j.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.logging.log4j</groupId>
                <artifactId>log4j-bom</artifactId>
                <version>${log4j2.version}</version>
                <scope>import</scope>
                <type>pom</type>
            </dependency>
            <dependency>
                <groupId>com.lmax</groupId>
                <artifactId>disruptor</artifactId>
                <version>${disruptor.version}</version>
            </dependency>

            <!-- Others -->
            <dependency>
                <groupId>com.alibaba</groupId>
                <artifactId>fastjson</artifactId>
                <version>${fastjson.version}</version>
            </dependency>
            <dependency>
                <groupId>com.google.code.gson</groupId>
                <artifactId>gson</artifactId>
                <version>${gson.version}</version>
            </dependency>
            <dependency>
                <groupId>io.netty</groupId>
                <artifactId>netty-all</artifactId>
                <version>${netty.version}</version>
            </dependency>
            <dependency>
                <groupId>org.projectlombok</groupId>
                <artifactId>lombok</artifactId>
                <version>${lombok.version}</version>
            </dependency>
        </dependencies>
    </dependencyManagement>

    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-source-plugin</artifactId>
                <version>${maven-source-plugin.version}</version>
                <executions>
                    <execution>
                        <id>attach-sources</id>
                        <goals>
                            <goal>jar</goal>
                        </goals>
                        <phase>package</phase>
                    </execution>
                </executions>
            </plugin>
        </plugins>
        <pluginManagement><!-- lock down plugins versions to avoid using Maven defaults (may be moved to parent pom) -->
            <plugins>
                <!-- clean lifecycle, see https://maven.apache.org/ref/current/maven-core/lifecycles.html#clean_Lifecycle -->
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-clean-plugin</artifactId>
                    <version>${maven-clean-plugin.version}</version>
                </plugin>
                <!-- default lifecycle, jar packaging: see https://maven.apache.org/ref/current/maven-core/default-bindings.html#Plugin_bindings_for_jar_packaging -->
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-resources-plugin</artifactId>
                    <version>${maven-resources-plugin.version}</version>
                    <configuration>
                        <encoding>${project.build.sourceEncoding}</encoding>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-compiler-plugin</artifactId>
                    <version>${maven-compiler-plugin.version}</version>
                    <configuration>
                        <source>${java.version}</source>
                        <target>${java.version}</target>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-javadoc-plugin</artifactId>
                    <version>${maven-javadoc-plugin.version}</version>
                    <executions>
                        <execution>
                            <id>javadoc</id>
                            <goals>
                                <goal>jar</goal>
                            </goals>
                            <phase>package</phase>
                        </execution>
                    </executions>
                    <configuration>
                        <failOnError>false</failOnError>
                        <quiet>true</quiet>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-failsafe-plugin</artifactId>
                    <version>${maven-failsafe-plugin.version}</version>
                    <executions>
                        <execution>
                            <goals>
                                <goal>integration-test</goal>
                                <goal>verify</goal>
                            </goals>
                        </execution>
                    </executions>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-surefire-plugin</artifactId>
                    <version>${maven-surefire-plugin.version}</version>
                    <configuration>
                        <includes>
                            <include>**/*Tests.java</include>
                            <include>**/*Test.java</include>
                        </includes>
                        <excludes>
                            <exclude>**/Abstract*.java</exclude>
                        </excludes>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-jar-plugin</artifactId>
                    <version>${maven-jar-plugin.version}</version>
                    <configuration>
                        <archive>
                            <manifest>
                                <mainClass>${start-class}</mainClass>
                                <addDefaultImplementationEntries>true</addDefaultImplementationEntries>
                            </manifest>
                        </archive>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-war-plugin</artifactId>
                    <version>${maven-war-plugin.version}</version>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-assembly-plugin</artifactId>
                    <version>${maven-assembly-plugin.version}</version>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-dependency-plugin</artifactId>
                    <version>${maven-dependency-plugin.version}</version>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-checkstyle-plugin</artifactId>
                    <version>${maven-checkstyle-plugin.version}</version>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-install-plugin</artifactId>
                    <version>${maven-install-plugin.version}</version>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-deploy-plugin</artifactId>
                    <version>${maven-deploy-plugin.version}</version>
                </plugin>
                <plugin>
                    <groupId>org.projectlombok</groupId>
                    <artifactId>lombok-maven-plugin</artifactId>
                    <version>${lombok-maven-plugin.version}</version>
                    <dependencies>
                        <dependency>
                            <groupId>org.projectlombok</groupId>
                            <artifactId>lombok</artifactId>
                            <version>${lombok.version}</version>
                        </dependency>
                    </dependencies>
                    <configuration>
                        <encoding>${project.build.sourceEncoding}</encoding>
                    </configuration>
                    <executions>
                        <execution>
                            <phase>generate-sources</phase>
                            <goals>
                                <goal>delombok</goal>
                            </goals>
                        </execution>
                    </executions>
                </plugin>
            </plugins>
        </pluginManagement>
    </build>

    <distributionManagement>
        <repository>
            <id>internal-maven</id>
            <name>RELEASE</name>
            <url>http://192.168.1.42:10090/repository/internal-release/</url>
        </repository>
        <snapshotRepository>
            <id>internal-maven-snapshots</id>
            <name>SNAPSHOT</name>
            <url>http://192.168.1.42:10090/repository/internal-snapshot/</url>
        </snapshotRepository>
    </distributionManagement>

    <repositories>
        <repository>
            <id>nexus</id>
            <name>local private nexus</name>
            <url>http://192.168.1.42:10090/repository/internal/</url>
            <releases>
                <enabled>true</enabled>
            </releases>
            <snapshots>
                <enabled>true</enabled>
            </snapshots>
        </repository>
    </repositories>

    <pluginRepositories>
        <pluginRepository>
            <id>nexus</id>
            <name>local private nexus</name>
            <url>http://192.168.1.42:10090/repository/internal/</url>
            <releases>
                <enabled>true</enabled>
            </releases>
            <snapshots>
                <enabled>true</enabled>
            </snapshots>
        </pluginRepository>
    </pluginRepositories>
</project>

```

## parent pom 定义

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

<!--    <parent>-->
<!--        <groupId>org.springframework.boot</groupId>-->
<!--        <artifactId>spring-boot-starter-parent</artifactId>-->
<!--        <version>2.1.1.RELEASE</version>-->
<!--        <relativePath/> &lt;!&ndash; lookup parent from repository &ndash;&gt;-->
<!--    </parent>-->

    <parent>
        <groupId>com.base</groupId>
        <artifactId>base-common-bom</artifactId>
        <version>1.0.0-SNAPSHOT</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>

    <modules>
        <module>demo-utils</module>
        <module>demo-common</module>
        <module>demo-db-app</module>
        <module>demo-remote-api</module>
    </modules>

    <groupId>com.base</groupId>
    <artifactId>demo-parent</artifactId>
    <version>1.0.0</version>
    <packaging>pom</packaging>

    <properties>
        <dubbo.version>2.6.2</dubbo.version>
        <dubbo.starter.version>0.2.0</dubbo.starter.version>
        <zookeeper.version>3.4.9</zookeeper.version>
        <curator.version>4.0.1</curator.version>
        <failOnMissingWebXml>false</failOnMissingWebXml>
        <swagger.version>2.6.0</swagger.version>
        <jjwt.version>0.9.0</jjwt.version>
        <gson.version>2.8.5</gson.version>
        <lombok.version>1.18.8</lombok.version>
        <poi.version>3.17</poi.version>
        <elasticsearch.version>7.8.0</elasticsearch.version>
    </properties>

    <dependencyManagement>
        <dependencies>
            <!-- demo项目公共模块 -->
            <dependency>
                <groupId>com.demo</groupId>
                <artifactId>demo-common</artifactId>
                <version>${project.version}</version>
            </dependency>
            <dependency>
                <groupId>com.demo</groupId>
                <artifactId>demo-utils</artifactId>
                <scope>compile</scope>
                <version>${project.version}</version>
            </dependency>

            <!-- Spring Boot Dubbo 依赖 -->
            <dependency>
                <groupId>com.alibaba.boot</groupId>
                <artifactId>dubbo-spring-boot-starter</artifactId>
                <version>${dubbo.starter.version}</version>
            </dependency>
            <!--J2EE-->
            <dependency>
                <groupId>javax.servlet</groupId>
                <artifactId>javax.servlet-api</artifactId>
                <version>3.1.0</version>
                <scope>provided</scope>
            </dependency>
            <!-- 矩阵计算包 -->
            <dependency>
                <groupId>org.ejml</groupId>
                <artifactId>ejml-all</artifactId>
                <version>0.37</version>
            </dependency>
            <!-- spring+dubbo配置 -->
            <dependency>
                <groupId>com.alibaba</groupId>
                <artifactId>dubbo</artifactId>
                <version>${dubbo.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.curator</groupId>
                <artifactId>curator-framework</artifactId>
                <version>${curator.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.zookeeper</groupId>
                <artifactId>zookeeper</artifactId>
                <version>${zookeeper.version}</version>
            </dependency>
            <dependency>
                <groupId>joda-time</groupId>
                <artifactId>joda-time</artifactId>
                <version>2.10</version>
            </dependency>
            <dependency>
                <groupId>Pingplusplus</groupId>
                <artifactId>pingpp-java</artifactId>
                <version>2.3.13</version>
            </dependency>
            <dependency>
                <groupId>org.bouncycastle</groupId>
                <artifactId>bcprov-ext-jdk16</artifactId>
                <version>1.45</version>
            </dependency>

            <dependency>
                <groupId>io.springfox</groupId>
                <artifactId>springfox-swagger2</artifactId>
                <version>${swagger.version}</version>
            </dependency>
            <dependency>
                <groupId>io.springfox</groupId>
                <artifactId>springfox-swagger-ui</artifactId>
                <version>${swagger.version}</version>
            </dependency>
            <dependency>
                <groupId>io.jsonwebtoken</groupId>
                <artifactId>jjwt</artifactId>
                <version>${jjwt.version}</version>
            </dependency>

            <dependency>
                <groupId>com.hankcs</groupId>
                <artifactId>hanlp</artifactId>
                <version>portable-1.7.7</version>
            </dependency>
            <!-- SM2,SM3,SM4依赖包 -->
            <dependency>
                <groupId>org.bouncycastle</groupId>
                <artifactId>bcprov-jdk16</artifactId>
                <version>1.46</version>
            </dependency>
            <!--解析Excel-->
            <dependency>
                <groupId>org.apache.poi</groupId>
                <artifactId>poi</artifactId>
                <version>${poi.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.poi</groupId>
                <artifactId>poi-ooxml</artifactId>
                <version>${poi.version}</version>
            </dependency>
            <dependency>
                <groupId>org.apache.poi</groupId>
                <artifactId>poi-ooxml-schemas</artifactId>
                <version>${poi.version}</version>
            </dependency>
            <dependency>
                <groupId>com.alibaba</groupId>
                <artifactId>easyexcel</artifactId>
                <version>2.2.6</version>
            </dependency>
            <!--解析Excel-->
            <dependency>
                <groupId>net.sourceforge.pinyin4j</groupId>
                <artifactId>pinyin4j</artifactId>
                <version>2.5.0</version>
            </dependency>
            <dependency>
                <groupId>com.cn.icethunder</groupId>
                <artifactId>tenson</artifactId>
                <version>1.1</version>
            </dependency>
            <!-- elasticsearch配置 -->
            <dependency>
                <groupId>org.elasticsearch</groupId>
                <artifactId>elasticsearch</artifactId>
                <version>${elasticsearch.version}</version>
            </dependency>
            <dependency>
                <groupId>org.elasticsearch.client</groupId>
                <artifactId>elasticsearch-rest-client</artifactId>
                <version>${elasticsearch.version}</version>
            </dependency>
            <dependency>
                <groupId>org.elasticsearch.client</groupId>
                <artifactId>transport</artifactId>
                <version>${elasticsearch.version}</version>
            </dependency>
            <dependency>
                <!-- required by elasticsearch -->
                <groupId>org.elasticsearch.plugin</groupId>
                <artifactId>transport-netty4-client</artifactId>
                <version>${elasticsearch.version}</version>
            </dependency>
            <dependency>
                <groupId>org.elasticsearch.client</groupId>
                <artifactId>elasticsearch-rest-high-level-client</artifactId>
                <version>${elasticsearch.version}</version>
            </dependency>
        </dependencies>
    </dependencyManagement>

    <build>
        <resources>
            <resource>
                <directory>${basedir}/src/main/resources</directory>
            </resource>
        </resources>
        <plugins>
            <plugin>
                <groupId>org.sonarsource.scanner.maven</groupId>
                <artifactId>sonar-maven-plugin</artifactId>
                <version>3.7.0.1746</version>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>8</source>
                    <target>8</target>
                    <encoding>UTF-8</encoding>
                    <compilerArgs>-Xlint:deprecation,-Xlint:unchecked</compilerArgs>
                </configuration>

            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-surefire-plugin</artifactId>
                <configuration>
                    <skipTests>true</skipTests>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-war-plugin</artifactId>
                <configuration>
                    <!--如果想在没有web.xml文件的情况下构建WAR，请设置为false。-->
                    <failOnMissingWebXml>false</failOnMissingWebXml>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-jar-plugin</artifactId>
                <configuration>
                    <excludes>
                        <exclude>**/*.properties</exclude>
                        <exclude>**/*.yml</exclude>
                        <exclude>**/log4j2.xml</exclude>
                    </excludes>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-deploy-plugin</artifactId>
                <configuration>
                    <skip>true</skip>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

## demo-db-app 子模块 pom 定义

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <parent>
        <groupId>com.base</groupId>
        <artifactId>demo-parent</artifactId>
        <version>1.0.0</version>
    </parent>

    <artifactId>demo-db-app</artifactId>
    <packaging>jar</packaging>

    <description>Demo db app project for Spring Boot</description>

    <dependencies>
        <!-- 公共模块 -->
        <dependency>
            <groupId>com.demo</groupId>
            <artifactId>demo-common</artifactId>
        </dependency>
        <dependency>
            <groupId>com.demo</groupId>
            <artifactId>demo-utils</artifactId>
        </dependency>
        <!-- Spring Boot Dubbo 依赖 -->
        <dependency>
            <groupId>com.alibaba.boot</groupId>
            <artifactId>dubbo-spring-boot-starter</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>org.springframework.boot</groupId>
                    <artifactId>spring-boot-starter-logging</artifactId>
                </exclusion>
                <exclusion>
                    <groupId>ch.qos.logback</groupId>
                    <artifactId>logback-classic</artifactId>
                </exclusion>
                <exclusion>
                    <groupId>commons-lang</groupId>
                    <artifactId>commons-lang</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <!-- Aliyun -->
        <dependency>
            <groupId>com.aliyun</groupId>
            <artifactId>aliyun-java-sdk-core</artifactId>
        </dependency>
        <dependency>
            <groupId>com.aliyun</groupId>
            <artifactId>aliyun-java-sdk-dysmsapi</artifactId>
        </dependency>
<!--        <dependency>-->
<!--            <groupId>org.springframework.cloud</groupId>-->
<!--            <artifactId>spring-cloud-starter-config</artifactId>-->
<!--        </dependency>-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-log4j2</artifactId>
        </dependency>
        <!-- log4j2异步日志依赖包 -->
        <dependency>
            <groupId>com.lmax</groupId>
            <artifactId>disruptor</artifactId>
        </dependency>
        <!-- 连接池配置 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <dependency>
            <groupId>org.mybatis.spring.boot</groupId>
            <artifactId>mybatis-spring-boot-starter</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>org.springframework.boot</groupId>
                    <artifactId>spring-boot-starter-logging</artifactId>
                </exclusion>
                <exclusion>
                    <groupId>ch.qos.logback</groupId>
                    <artifactId>logback-classic</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>com.github.pagehelper</groupId>
            <artifactId>pagehelper-spring-boot-starter</artifactId>
        </dependency>

        <!--aspectj支持-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-aop</artifactId>
        </dependency>
        <!-- activeMQ 依赖 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-activemq</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>org.springframework.boot</groupId>
                    <artifactId>spring-boot-starter-logging</artifactId>
                </exclusion>
                <exclusion>
                    <groupId>ch.qos.logback</groupId>
                    <artifactId>logback-classic</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>joda-time</groupId>
            <artifactId>joda-time</artifactId>
        </dependency>
        <dependency>
            <groupId>com.google.code.gson</groupId>
            <artifactId>gson</artifactId>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-collections4</artifactId>
        </dependency>
        <dependency>
            <groupId>commons-codec</groupId>
            <artifactId>commons-codec</artifactId>
        </dependency>
        <dependency>
            <groupId>Pingplusplus</groupId>
            <artifactId>pingpp-java</artifactId>
        </dependency>
        <dependency>
            <groupId>com.hankcs</groupId>
            <artifactId>hanlp</artifactId>
        </dependency>
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
        </dependency>
        <!-- Redis配置 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-redis</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>org.springframework.boot</groupId>
                    <artifactId>spring-boot-starter-logging</artifactId>
                </exclusion>
                <exclusion>
                    <groupId>ch.qos.logback</groupId>
                    <artifactId>logback-classic</artifactId>
                </exclusion>
            </exclusions>
        </dependency>

        <!-- elasticsearch配置 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-elasticsearch</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>org.springframework.data</groupId>
                    <artifactId>spring-data-elasticsearch</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>org.springframework.data</groupId>
            <artifactId>spring-data-elasticsearch</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>org.elasticsearch.client</groupId>
                    <artifactId>elasticsearch-rest-high-level-client</artifactId>
                </exclusion>
                <exclusion>
                    <groupId>org.elasticsearch.client</groupId>
                    <artifactId>transport</artifactId>
                </exclusion>
                <exclusion>
                    <groupId>org.elasticsearch.plugin</groupId>
                    <artifactId>transport-netty4-client</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>org.elasticsearch</groupId>
            <artifactId>elasticsearch</artifactId>
        </dependency>
        <dependency>
            <groupId>org.elasticsearch.client</groupId>
            <artifactId>elasticsearch-rest-client</artifactId>
        </dependency>
        <dependency>
            <groupId>org.elasticsearch.client</groupId>
            <artifactId>transport</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>commons-logging</groupId>
                    <artifactId>commons-logging</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <!-- required by elasticsearch -->
            <groupId>org.elasticsearch.plugin</groupId>
            <artifactId>transport-netty4-client</artifactId>
        </dependency>
        <dependency>
            <groupId>org.elasticsearch.client</groupId>
            <artifactId>elasticsearch-rest-high-level-client</artifactId>
            <exclusions>
                <exclusion>
                    <groupId>commons-logging</groupId>
                    <artifactId>commons-logging</artifactId>
                </exclusion>
            </exclusions>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>

        <dependency>
            <groupId>com.aliyun.oss</groupId>
            <artifactId>aliyun-sdk-oss</artifactId>
            <version>3.1.0</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
        </dependency>
    </dependencies>

    <profiles>
        <profile>
            <id>dev</id>
            <properties>
                <activeEnv>dev</activeEnv>
            </properties>
            <activation>
                <activeByDefault>true</activeByDefault>
            </activation>
        </profile>
        <profile>
            <id>test</id>
            <properties>
                <activeEnv>test</activeEnv>
            </properties>
        </profile>
        <profile>
            <id>stage</id>
            <properties>
                <activeEnv>stage</activeEnv>
            </properties>
        </profile>
        <profile>
            <id>prod</id>
            <properties>
                <activeEnv>prod</activeEnv>
            </properties>
        </profile>
    </profiles>

    <build>
        <!-- 打包后的文件名 -->
        <finalName>${artifactId}</finalName>
        <resources>
            <!-- 指定默认配置资源 -->
            <resource>
                <directory>src/main/resources</directory>
                <filtering>true</filtering>
            </resource>
            <!-- 指定profile配置文件 -->
            <resource>
                <directory>src/profiles/${activeEnv}</directory>
                <filtering>true</filtering>
            </resource>
        </resources>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-jar-plugin</artifactId>
                <configuration>
                    <archive>
                        <manifest>
                            <!--指定main入口 -->
                            <mainClass>com.demo.db.app.DbAppApplication</mainClass>
                            <!-- 在Manifest中增加Class-Path项 -->
                            <addClasspath>true</addClasspath>
                            <!-- 指定Class-Path的前缀 -->
                            <classpathPrefix>lib/</classpathPrefix>
                        </manifest>
                        <manifestEntries>
                            <!-- Class-Path的增加当前路径，以便config文件夹中的配置文件可以被识别 -->
                            <Class-Path>./</Class-Path>
                        </manifestEntries>
                    </archive>
                    <excludes>
                        <exclude>config/**</exclude>
                    </excludes>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-assembly-plugin</artifactId>
                <configuration>
                    <encoding>${project.build.sourceEncoding}</encoding>
                    <appendAssemblyId>true</appendAssemblyId>
                    <descriptors>
                        <descriptor>src/assembly/assembly.xml</descriptor>
                    </descriptors>
                </configuration>
                <executions>
                    <execution>
                        <id>make-assembly</id>
                        <phase>package</phase>
                        <goals>
                            <goal>single</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>

</project>

```

### 配置分离内容

```xml
<assembly xmlns="http://maven.apache.org/ASSEMBLY/2.1.0"
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
          xsi:schemaLocation="http://maven.apache.org/ASSEMBLY/2.1.0
          http://maven.apache.org/xsd/assembly-2.1.0.xsd">
    <id>deploy</id>
    <formats>
        <format>zip</format>
    </formats>
    <includeBaseDirectory>false</includeBaseDirectory>
    <baseDirectory>/</baseDirectory>

    <fileSets>
        <!-- 可执行jar包 -->
        <fileSet>
            <directory>${project.build.directory}</directory>
            <outputDirectory>${artifactId}/</outputDirectory>
            <includes>
                <include>${build.finalName}.jar</include>
            </includes>
            <fileMode>755</fileMode>
        </fileSet>
        <!-- 配置文件输出到config目录下 -->
        <fileSet>
            <!-- ${profiles.dir}变量在bom中已定义，${activeEnv}取自父pom的profiles定义 -->
            <directory>${profiles.dir}/${activeEnv}/config</directory>
            <outputDirectory>${artifactId}/config</outputDirectory>
            <fileMode>755</fileMode>
            <filtered>true</filtered>
        </fileSet>
    </fileSets>

    <dependencySets>
        <!-- 依赖jar包输出到lib目录下 -->
        <dependencySet>
            <outputDirectory>${artifactId}/lib</outputDirectory>
            <scope>runtime</scope>
            <excludes>
                <exclude>${groupId}:${artifactId}</exclude>
            </excludes>
        </dependencySet>
    </dependencySets>

</assembly>
```
