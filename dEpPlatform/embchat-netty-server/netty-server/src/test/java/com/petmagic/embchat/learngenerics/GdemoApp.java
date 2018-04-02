package com.petmagic.embchat.learngenerics;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

/**
 * 泛型学习
 * Created by c on 15/7/14.
 */
public class GdemoApp {
    public static void main(String[] args) {
        Gdemo g = new Gdemo();
        g.print(1);
        g.print("sdfsdf");
        g.print(0.22);

        List<String> ss = new ArrayList<String>();
        ss.add("我是中国人");
        g.printList(ss);

        List<Integer> ii = new ArrayList<Integer>();
        ii.add(7);
        g.printList(ii);

    }
}
class Gdemo {

    /**
     * 实现多态 参数可传任意Object
     * @param t
     * @param <T>
     */
    public<T> void print(T t) {
        System.out.println(t);
    }

    /**
     * 通配符?
     * 如果仅仅实现多态 优先使用通配符
     * @param s
     */
    public void printList(List<?> s) {
        for (Object o : s) {
            System.out.println(o);
        }
    }

}

//Class支持泛型    实例
class executeQuery {
    public void exeq() {
        try {
            Collection<Example> collection = SqlUtil.select(Example.class, "");
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        }
    }
}
class SqlUtil {
    public static <T> Collection<T> select(Class<T> c, String sql) throws IllegalAccessException, InstantiationException {
        Collection<T> result = new ArrayList<T>();
        // step 1 run sql
        //for (iterate over jdbc results) {
            T item = c.newInstance();
        //}
        result.add(item);
        return result;
    }
}
class Example {
    int id;
    String name;
}
