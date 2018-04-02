package com.petmagic.embchat.learnio;

/**
 * author:     li.weichao
 * revision:   #
 * date:       2015/8/8 16:35
 */
public class Member {
    private String name;
    private int age;
    public Member() {
    }
    public Member(String name, int age) {
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
}
