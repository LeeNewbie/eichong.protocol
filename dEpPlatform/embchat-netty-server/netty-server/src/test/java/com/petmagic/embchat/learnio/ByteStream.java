package com.petmagic.embchat.learnio;

import java.io.*;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Vector;

/**
 * 字节流
 * Author:     lewec
 * Revision:   #
 * Date:       2015/8/8 15:35
 */
public class ByteStream {

    public static void main(String[] args) {
        FileCount fileCount = new FileCount();
        fileCount.count();

        FileCopy fileCopy = new FileCopy();
        fileCopy.copy();

        ObjectStream objectStream = new ObjectStream();
        objectStream.object();

        DataStream dataStream = new DataStream();
        dataStream.data();
        System.out.println("===================================");
        PushBackStream backStream = new PushBackStream();
        backStream.pushBack();
        System.out.println("===================================");
        SequenceStream sequenceStream = new SequenceStream();
        sequenceStream.doSequence();
    }

}

/**
 * 无缓冲方式
 */
class FileCount {
    public void count() {
        int count = 0; //统计文件字节长度
        InputStream inputStream = null;
        try {
            inputStream = new FileInputStream(new File("D:/JAVATEST/呼叫中心1.jpg"));

            // 从inputStream中读取一个字节
            // 返回值是下一个数据字节，如果已经达到文件末尾 返回-1
            while(inputStream.read() != -1) {
                count++;
            }
            System.out.println("--长度是： " + count + " 字节");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                //FileInputStream是有缓冲区的，所以用完后必须关闭，否则可能导致内存占满数据丢失
                inputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

/**
 * 使用缓冲方式
 * 所谓缓冲 就是先把从流中得到的一块字节序列暂存到一个数组里，
 * 然后你可以一下子取到这一整块的字节数据
 * 没有缓冲的流只能一个字节一个字节的读，效率相对偏低
 * JAVA I/O默认是不缓冲流的（下面自己实现缓冲） 但是有两个特殊的输入流实现了
 * 缓冲功能，一个是常用的BufferedInputStream
 *
 */
class FileCopy {
    public void copy() {
        byte[] buffer = new byte[512]; //一次取出的字节数大小,缓冲区大小
        int numberRead = 0;
        FileInputStream inputStream = null;
        FileOutputStream outputStream = null;

        try {
            inputStream = new FileInputStream("D:/JAVATEST/呼叫中心1.jpg");

            //如果文件不存在 会自动创建
            outputStream = new FileOutputStream("D:/JAVATEST/呼叫中心1_temp.jpg");

            // 从inputStream中读取最多buffer.length个字节存入buffer中
            // 返回值是 读取到的字节总数 如果已经达到文件末尾 返回-1
            while ((numberRead = inputStream.read(buffer)) != -1) {

                //将缓冲区buffer中 从偏移量0位置开始的 numberRead个字节写入输出流outputStream
                outputStream.write(buffer, 0, numberRead);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                inputStream.close();
                outputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

/**
 * 读写对象
 * ObjectInputStream 和 ObjectOutputStream 允许读取或写入用户自定义的类，
 * 但是被读取和写入的类必须实现Serializable接口
 * Serializable 序列化 用于保存内存中的对象状态
 */
class ObjectStream {
    public void object() {
        ObjectOutputStream objectOutputStream = null;
        ObjectInputStream objectInputStream = null;

        try {
            objectOutputStream = new ObjectOutputStream(new FileOutputStream("D:/JAVATEST/objectstream.txt"));
            objectOutputStream.writeObject(new Student("gg", 22));
            objectOutputStream.writeObject(new Student("tt", 18));
            objectOutputStream.writeObject(new Student("rr", 17));
            objectInputStream = new ObjectInputStream(new FileInputStream("D:/JAVATEST/objectstream.txt"));
            for (int i = 0; i < 3; i++) {
                System.out.println(objectInputStream.readObject());
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } finally {
            try {
                objectInputStream.close();
                objectOutputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

/**
 * DataInputStream DataOutputStream 继承自FilterInputStream FilterOutputStream
 * 有时候没必要存储整个对象的信息，而只是要存储一个对象的成员数据，
 * 假设成员数据的类型都是JAVA基础类型，(这个假定是为了避免使用到Object输入输出流)，
 * 就可以使用DataInputStream DataOutPutStream来读出和写入数据。
 * DataInputStream的好处在于从文件读出数据时，不必自行判断读入字符串或读入int
 * 时何时将停止，使用对应的readUTF()和readInt()方法就可以正确的读入完整的类型数据
 */
class DataStream {
    Member[] members = {
            new Member("Justin", 90),
            new Member("Momor", 95),
            new Member("Bush", 88)
    };

    public void data() {
        DataOutputStream dataOutputStream = null;
        DataInputStream dataInputStream = null;
        try {
            dataOutputStream = new DataOutputStream(new FileOutputStream("D:/JAVATEST/datastream.txt"));
            for(Member member : members) {
                //写入UTF字符串
                dataOutputStream.writeUTF(member.getName());
                //写入int数据
                dataOutputStream.writeInt(member.getAge());
            }
            //强制写入缓冲中的字节数据，并清空输出流
            dataOutputStream.flush();
            dataInputStream = new DataInputStream(new FileInputStream("D:/JAVATEST/datastream.txt"));

            //读入并还原为对象
            for (int i = 0; i < members.length; i++) {
                //读出UTF字符串
                String name = dataInputStream.readUTF();
                //读出int数据
                int score = dataInputStream.readInt();
                members[i] = new Member(name, score);
            }

            for (Member member : members) {
                System.out.printf("%s\t%d%n", member.getName(), member.getAge());
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                dataOutputStream.close();
                dataInputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

/**
 * PushbackInputStream继承自FilterInputStream
 * 提供可以将数据插入到输入流前端的能力（当然还有其他操作），
 * 简而言之 PushbackInputStream类的作用就是能够在读取缓冲区的时候提前知道下一个字节是什么，
 * 其本质是读取到下一个字符后回退
 * 如果要插入 能够插入的最大字节数是与回退缓冲区大小相关的，插入的字符肯定不能大于缓冲区。。
 */
class PushBackStream {

    public void pushBack() {
        String str = "hello,rollenholt";
        PushbackInputStream pushbackInputStream = null;//声明回退流对象
        ByteArrayInputStream bat = null;   //声明字节数组流对象 ByteArrayInputStream 无需抛异常

        bat = new ByteArrayInputStream(str.getBytes());
        pushbackInputStream = new PushbackInputStream(bat);
        int temp = 0;
        try {
            // push.read() 逐字节读取存放在temp中， 如果度去完成 返回-1
            while ((temp = pushbackInputStream.read()) != -1) {
                if(temp == ',') {
                    pushbackInputStream.unread(temp); //回到temp的位置
                    temp = pushbackInputStream.read();//接着读取字节
                    System.out.println("(回退" + (char)temp + ")"); //输出回退的字符
                } else {
                    System.out.println((char) temp);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

/**
 * SequenceInputStream  SequenceOutputStream
 * 当我们需要从多个输入流中向程序读入数据，此时可以使用合并流，
 * 将多个输入流合并成一个SequenceInputStream流对象，
 * SequenceInputStream会将与之相连接的流集组合成一个输入流并从第一个输入流开始读取，
 * 知道到达文件末尾，接着从第二个输入流读取，依次类推，知道 到达包含的最后一个输入流的文件末尾
 * 为止。
 * 合并流的作用是将多个源合并一个源，其可接收美剧类所封闭的多个字节流对象
 */
class SequenceStream {
    public void doSequence() {
        SequenceInputStream sis = null;
        BufferedOutputStream bos = null;
        //构建流集合
        Vector<InputStream> vector = new Vector<InputStream>();
        try {
            vector.addElement(new FileInputStream("D:/JAVATEST/text1.txt"));
            vector.addElement(new FileInputStream("D:/JAVATEST/text2.txt"));
            vector.addElement(new FileInputStream("D:/JAVATEST/text3.txt"));
            Enumeration<InputStream> e = vector.elements();

            sis = new SequenceInputStream(e);

            bos = new BufferedOutputStream(new FileOutputStream("D:/JAVATEST/text4.txt"));

            //读写数据
            byte[] buf = new byte[1024];
            int len = 0;

            while ((len = sis.read(buf)) != -1) {
                bos.write(buf, 0, len);
                bos.flush();
            }


        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                sis.close();
                bos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}


// PrintStream相当于 System.Out.Print(); 不过多示例