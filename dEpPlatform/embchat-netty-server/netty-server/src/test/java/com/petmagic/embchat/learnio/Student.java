package com.petmagic.embchat.learnio;

import java.io.Serializable;

/**
 * author:     li.weichao
 * revision:   #
 * date:       2015/8/8 16:10
 */
public class Student implements Serializable {
    private String name;
    private int age;
    public Student(String name, int age) {
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return "Student [name=" + name + ",age=" + age + "]";
    }
}
