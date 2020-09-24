# Redis同步本地缓存的定时任务

## 定义Redis操作的Service和实现

Service定义：

```java
public interface DataCacheForService {

    /**
     * 增加干扰词到集合中
     *
     * @param terms
     * @return
     */
    boolean addNoisedTerms(Collection<String> terms);

    /**
     * 获取所有的干扰词集合
     *
     * @return
     */
    Set<String> getAllNoisedTerms();

    /**
     * 设置干扰词更新的集合批次
     *
     * @param timestamp value为设置时的时间戳
     * @param timeout 超时时间
     * @param unit 时间单位
     * @return
     */
    void setNoisedTermsUpdate(long timestamp, long timeout, TimeUnit unit);

    /**
     * 获取菜品干扰词更新的集合批次
     *
     * @return
     */
    Object getNoisedTermsUpdate();
}
```

Service实现代码：

```java
@Service
public class DataCacheForServiceImpl implements DataCacheForService {

    /**
     * 应用缓存前缀
     */
    public static final String REDIS_PROJECT_PREFIX = "DEMO:";

    /**
     * 干扰词集合set的key
     * key="DEMO:NOISYTERMS:FOOD"
     */
    public static final String CACHE_NOISY_TERMS_FOOD_KEY = REDIS_PROJECT_PREFIX + "NOISYTERMS:FOOD";

    /**
     * 干扰词集合更新批次的key
     * key="DEMO:NOISYTERMS:FOOD:UPDATE"
     * value=时间戳
     */
    public static final String CACHE_NOISY_TERMS_FOOD_UPDATE_KEY = CACHE_NOISY_TERMS_FOOD_KEY + ":UPDATE";

    @Autowired
    RedisTemplate redisTemplate;

    @Override
    public boolean addNoisedTerms(Collection<String> terms) {
        if (CollectionUtils.isEmpty(terms)){
            return false;
        }

        Long result = redisTemplate.opsForSet().add(CACHE_NOISY_TERMS_FOOD_KEY, terms.toArray());
        return result !=null && result > 0;
    }

    @Override
    public Set<String> getAllNoisedTerms() {
        return redisTemplate.opsForSet().members(CACHE_NOISY_TERMS_FOOD_KEY);
    }

    @Override
    public void setNoisedTermsUpdate(long timestamp, long timeout, TimeUnit unit) {
        redisTemplate.opsForValue().set(CACHE_NOISY_TERMS_FOOD_UPDATE_KEY, timestamp, timeout, unit);
    }

    @Override
    public Object getNoisedTermsUpdate() {
        return redisTemplate.opsForValue().get(CACHE_NOISY_TERMS_FOOD_UPDATE_KEY);
    }
}
```

## 定义本地缓存类

本地缓存类：

```java
/**
 * 干扰词缓存
 *
 */
public class NoisedTermsCache {

    /**
     * 缓存干扰词的集合
     */
    private HashSet<String> hashSet = new HashSet<>();

    // 单例模式
    private NoisedTermsCache() {

    }

    private static class NoisedTermsCacheHolder {
        public static final NoisedTermsCache instance = new NoisedTermsCache();
    }

    public static NoisedTermsCache getInstance() {
        return NoisedTermsCacheHolder.instance;
    }

    /**
     * 初始化干扰词的集合缓存
     */
    public void init(Collection<String> noisedTermsSet) {
        if (CollectionUtils.isNotEmpty(noisedTermsSet)) {
            hashSet.addAll(noisedTermsSet);
        }
    }

    /**
     * 清空干扰词的集合缓存
     */
    public void clear() {
        hashSet.clear();
    }

    /**
     * 更新缓存
     * @param noisedTermsSet
     */
    public void updateCache(Collection<String> noisedTermsSet) {
        if (hashSet.equals(noisedTermsSet)) {
            return;
        }

        // 清空本地缓存
        hashSet.clear();
        // 更新本地缓存
        if (CollectionUtils.isNotEmpty(noisedTermsSet)) {
            hashSet.addAll(noisedTermsSet);
        }
    }

    /**
     * 缓存中是否包含该词项
     *
     * @param term
     * @return
     */
    public boolean contains(String term) {
        return hashSet.contains(term);
    }

}
```

## 同步本地缓存的定时服务

同步本地缓存的定时服务是`Runnable`接口的实现，主线程通过启动新线程的方式调用该服务：

```java
/**
 * 同步干扰词集合缓存定时缓存
 */
public class InitNoisedTermsWorker implements Runnable {

    private static final Logger LOGGER = LoggerFactory.getLogger(InitNoisedTermsWorker.class);

    // 默认执行间隔时间60s
    private static final long DEFAULT_EXECUTE_SPAN = 60 * 1000;

    // 本地缓存是否已初始化
    private volatile boolean inited = false;

    // 服务取行标识
    private volatile boolean cancelled = false;

    // 本地缓存更新的批次号
    private String localNoisedTermsUpdateKey = String.valueOf(System.currentTimeMillis());

    // 服务执行的时间间隔
    private long executeSpan = DEFAULT_EXECUTE_SPAN;


    private final NoisedTermsCache noisedTermsCache;

    private final DataCacheForEsService dataCacheForEsService;

    public InitFoodNoisedTermsWorker(NoisedTermsCache noisedTermsCache, DataCacheForEsService dataCacheForEsService) {
        this.noisedTermsCache = noisedTermsCache;
        this.dataCacheForEsService = dataCacheForEsService;
    }

    /**
     * 1. 本地缓存未初始化直接从redis获取缓存集合更新本地缓存
     *
     * 2. 本地缓存已初始化
     * 2.1 获取redis干扰词更新批次号，判断更新批次号已存在，且不等于本地缓存的上次更新批次号
     * 2.2 更新本地缓存
     * 2.3 缓存本次更新的批次号
     */
    @Override
    public void run() {
        long stime;
        while (!cancelled && !Thread.interrupted()) {

            if (!inited) {
                stime = System.currentTimeMillis();
                // 初始化缓存
                updateLocalCache();
                inited = true;
                LOGGER.info("[appInit]初始化干扰词集合缓存，耗时：{} 毫秒", System.currentTimeMillis() - stime);
            } else {
                // 检查更新，获取缓存更新批次号
                Object updateFlag = dataCacheForEsService.getFoodNoisedTermsUpdate();
                // 验证是否已初始化或本次更新是否已同步到本地缓存
                // 更新批次号为时间戳，且10分钟过期
                if (updateFlag != null && !localNoisedTermsUpdateKey.equals(updateFlag.toString())) {
                    LOGGER.info("[appInit]更新干扰词集合缓存，已更新批次为：{}，将要更新批次为：{} ",
                            localNoisedTermsUpdateKey, updateFlag);
                    stime = System.currentTimeMillis();
                    // 更新缓存
                    updateLocalCache();

                    // 更新本地缓存已更新的标识
                    localNoisedTermsUpdateKey = updateFlag.toString();
                    LOGGER.info("[appInit]更新干扰词集合缓存，更新批次为：{}，耗时：{} 毫秒",
                            localNoisedTermsUpdateKey, System.currentTimeMillis() - stime);
                }
            }

            // 设置线程休眠时间，保证1分钟获取一次
            try {
                Thread.sleep(executeSpan);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    private void updateLocalCache() {
        // 从redis获取缓存信息
        Set<String> foodNoisedTermsSet = dataCacheForEsService.getAllFoodNoisedTerms();
        // 更新本地缓存
        noisedTermsCache.updateCache(foodNoisedTermsSet);
        LOGGER.info("[appInit]更新干扰词集合缓存，term数量：{} 个，内容：{}",
                foodNoisedTermsSet == null ? 0 : foodNoisedTermsSet.size(), foodNoisedTermsSet);
    }

    public boolean isCancelled() {
        return cancelled;
    }

    public void setCancelled(boolean cancelled) {
        this.cancelled = cancelled;
    }

    public long getExecuteSpan() {
        return executeSpan;
    }

    public void setExecuteSpan(long executeSpan) {
        this.executeSpan = executeSpan;
    }
}
```

## 主线程启动服务

主线程启动服务代码

```java    
    Thread initNoisedTermsWorker = new Thread(new InitNoisedTermsWorker(FoodNoisedTermsCache.getInstance(),new DataCacheForEsService));
    initNoisedTermsWorker.start();
```

## Redis缓存集合更新的Controller

```java
    @ApiOperation(value = "增加搜索干扰词", notes = "增加搜索干扰词，多个干扰词之间用英文逗号分隔，中间不可以有空格")
    @ApiImplicitParams({@ApiImplicitParam(name = "terms", value = "terms", required = true, paramType = "body", defaultValue = "")
    })
    @PostMapping(value = "/noisy/terms")
    public String addNoisedTerms(@RequestBody String terms) {
        if (StringUtils.isBlank(terms)) {
            return "PARAMETER IS NULL";
        }

        dataCacheForEsService.addFoodNoisedTerms(Arrays.asList(terms.split(",")));
        // 设置更新批次且10分钟过期
        dataCacheForEsService.setFoodNoisedTermsUpdate(System.currentTimeMillis(), 10, TimeUnit.MINUTES);

        return "OK";
    }
```
