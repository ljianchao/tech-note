# mysql批量插入

## 代码示例

定义service类

```java
public interface UserBatchService {

    /**
     * 批量插入，采用MyBatis的Executor.BATCH模式
     *
     * @param userList 用户列表
     */
    void batchInsert(List<User> userList);
}
```

定义实现类

```java
@Service
public class UserBatchServiceImpl implements UserBatchService {

    @Autowired
    SqlSessionFactory sqlSessionFactory;

    @Override
    public void batchInsert(List<User> userList) {
        if (CollectionUtils.isEmpty(userList)) {
            return;
        }

        try (SqlSession sqlSession = sqlSessionFactory.openSession(ExecutorType.BATCH)) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            userList.forEach(mapper::insertUser);

            sqlSession.commit();
        }
    }
}
```

mapper定义

```java
public interface UserMapper {
    int insertUser(User user);
}
```

mapper.xml定义

```xml
<insert id="insertUser" parameterType="com.demo.user">
    insert into user (user_id, user_name)
    values (#{userId}, #{userName})
</insert>
```

