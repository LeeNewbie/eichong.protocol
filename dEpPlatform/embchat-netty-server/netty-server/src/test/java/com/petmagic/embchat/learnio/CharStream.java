package com.petmagic.embchat.learnio;

import java.io.*;

/**
 * 字符流  操作字符文件的流
 * author:     li.weichao
 * revision:   #
 * date:       2015/8/8 19:00
 */
public class CharStream {
    public static void main(String[] args) {
        CharReader charReader = new CharReader();
        charReader.charr();

        Freader freader = new Freader();
        freader.print();

        BufferedRW bufferedRW = new BufferedRW();
        bufferedRW.concennateFile("D:/JAVATEST/text1.txt","D:/JAVATEST/text2.txt","D:/JAVATEST/text3.txt","D:/JAVATEST/copy1.txt");

        StreamTokenizerDemo streamTokenizerDemo = new StreamTokenizerDemo();
        long num = streamTokenizerDemo.statis("D:/JAVATEST/article.txt");
        System.out.println(num);
    }
}

/**
 * CharArrayReader 对应字节操作类是 ByteArrayInputStream
 */
class CharReader {
    public void charr() {
        String str = "hello world";
        CharArrayReader reader = new CharArrayReader(str.toCharArray());
        try {
            int r = 0;
            while ((r = reader.read()) != -1) {
                System.out.println((char) r);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

class Freader {
    public void print() {
        char[] buffer = new char[512];
        int numberRead = 0;
        FileReader reader = null;   //读取字符文件的流
        PrintWriter writer = null;
        try {
            reader = new FileReader("D:/JAVATEST/copy1.txt");
            writer = new PrintWriter(System.out);   //输出到控制台的流
            while ((numberRead = reader.read(buffer)) != -1) {
                writer.write(buffer, 0, numberRead);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                reader.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            writer.close();
        }
    }
}

/**
 * BufferedReader BufferedWriter
 * 自带缓冲区， 最常用的方法是readLine(); 读取一行数据 并返回String
 */
class BufferedRW {
    public void concennateFile(String... fileName) {
        String str;
        BufferedWriter writer = null;
        BufferedReader reader = null;
        try {
            writer = new BufferedWriter(new FileWriter("D:/JAVATEST/copy2.txt"));
            for (String name : fileName) {
                reader = new BufferedReader(new FileReader(name));

                while ((str = reader.readLine()) != null) {
                    writer.write(str);
                    writer.newLine();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

// Console类，该类提供了用于读取密码的方法，可以禁止控制台回显并返回char数组，
// 这两个特性对保证安全有作用，平时用的不多，作为了解即可


/**
 * StreamTokenizer
 * 这个类非常有用，它可以把输入流解析为标记(token)， StreamTokenizer并非派生自InputStream
 * 或者OutputStream， 而是归类于io库中， 因为StreamTokenizer只处理InputStream对象。
 * 首先给出我的文本文件内容
 * '水上漂'
 * 青青草
 * "i love wyhss"
 * {3211}
 * 23223 3523
 * i love wyh ，。
 * .,
 *
 * 使用StreamTokenizer来统计文件中的字符数
 * StreamTokenizer类获取输入流 并将其分析为“标记”，允许一次读取一个标记。
 * 分析过程由一个表和许多可以设置为各种状态的标志控制
 * 该流的标记生成器可以识别标识符、数字、引用的字符串和各种注释样式。
 *
 * 默认情况下，StreamTokenizer认为下列内容是Token:字母、数字、除C和C++注释符号以外的其他符号。
 * 如符号"/"不是token， 注释后的内容也不是，而"\"是Token。单引号和双引号以及其中的内容，只能算
 * 是一个Token。
 *
 * 统计文章字符数的程序，不是简单的统计Token数就万事大吉，因为字符数不等于Token。按照Token的规定，
 * 引号中的内容就算是10页也算是一个Token。如果希望引号和引号中的内容都算作Token，应该调用下面的代
 * 码：st.ordinaryChar(''\');
 *    st.ordinaryChar('\"');
 *
 */
class StreamTokenizerDemo {
    public static void main(String[] args) {
        StreamTokenizerDemo streamTokenizerDemo = new StreamTokenizerDemo();
        streamTokenizerDemo.statis("D:/JAVATEST/article.txt");

    }
    public long statis(String fileName) {
        FileReader fileReader = null;
        try {
            fileReader = new FileReader(fileName);

            //创建分析给定字符流的标记生成器
            StreamTokenizer st = new StreamTokenizer(new BufferedReader(fileReader));

            //ordinaryChar方法指定字符参数在此标记生成器中是“普通”字符。
            st.ordinaryChar('\'');
            st.ordinaryChar('\"');
            st.ordinaryChar('/');

            String s;
            int numberSum = 0;
            int wordSum = 0;
            int symbolSum = 0;
            int total = 0;
            //nextToken方法读取下一个Token.
            //TT_EOF指示已读到流末尾的常量
            while (st.nextToken() != StreamTokenizer.TT_EOF) {
                //在调用 nextToken方法之后，ttype字段将包含刚读取的标记的类型
                switch (st.ttype) {
                    //TT_EOF指示已读到行末尾的常量
                    case StreamTokenizer.TT_EOF:
                        break;
                    //TT_NUMBER指示已读到一个数字标记的常量
                    case StreamTokenizer.TT_NUMBER:
                        //如果当前标记是一个数字，nval字段将包含该数字的值
                        s = String.valueOf(st.nval);
                        System.out.println("数字有： "+s);
                        numberSum ++;
                        break;
                    //TT_WORD指示已读到一个文字标记的常量
                    case StreamTokenizer.TT_WORD:
                        //如果当前标记是一个文字标记，sval字段包含一个给出该文字标记的字符的字符串
                        s = st.sval;
                        System.out.println("单词有：" + s);
                        wordSum ++;
                        break;
                    default:
                        //如果以上3种类型都不是，则为英文的标点符号
                        s = String.valueOf((char) st.ttype);
                        System.out.println("标点有：" + s);
                        symbolSum ++;
                }
            }

            System.out.println("数字有 " + numberSum + "个");
            System.out.println("单词有 " + wordSum + "个");
            System.out.println("标点符号有 " + symbolSum + "个");
            total = symbolSum + numberSum + wordSum;
            System.out.println("Total = " + total);
            return total;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return -1;
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        } finally {
            if(fileReader != null) {
                try {
                    fileReader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /*我们从其中可以看到很多东西：
     1.一个单独的小数点“.”是被当做一个数字来对待的，数字的值为0.0；
     2.一串汉字只要中间没有符号（空格回车 分号等等）都是被当做一个单词的。中文的标点跟中文的汉字一样处理
     3.如果不对引号化成普通字符，一个引号内的内容不论多少都被当做是一个标记。
     4.该类能够识别英文标点
     6. java io里面还有其他接口类似Serializable接口的子接口Externalizable接口，
        比Serializable复杂一些，这里不再介绍。还有关于java对象版本化的东西感兴趣的可以百度。
        java nio的东西这里没有涉及，后续会结合到线程再发一篇文章专门解析这个东西。*/
}