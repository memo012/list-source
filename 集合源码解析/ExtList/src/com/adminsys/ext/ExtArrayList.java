package com.adminsys.ext;

import java.util.Arrays;

/**
 * @Author: qiang
 * @Description: ArrayList集合
 * @Create: 2019-12-11 11-51
 **/

public class ExtArrayList<E> implements ExtList<E> {

    /**
     *  elementData数据存放我们ArrayList所有的数据
     *  transient 非序列化
     */
    transient Object[] elementData;

    /**
     *  给我们的数组容量赋值空
     */
    private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};

    /**
     *  数组的容量默认大小为0
     */
    private int size;


    /**
     *  数组默认容量大小
     */
    private static final int DEFAULT_CAPACITY = 10;
    /**
     *  2的32次方减8
     */
    private static final int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean add(E e) {
        // 对数组实现扩容
        ensureCapacityInternal(size + 1);
        // 对我们的数据的元素赋值
        elementData[size++] = e;
        return true;
    }

    @Override
    public E get(int index) {
        // 查询数据
        return (E)elementData[index];
    }

    public ExtArrayList() {
        this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
    }

    private void ensureCapacityInternal(int minCapacity){
        ensureExplicitCapacity(calculateCapacity(elementData, minCapacity));
    }

    /**
     *  给数组确定容量
     * @param elementData 数组
     * @param minCapacity
     * @return
     */
    private static int calculateCapacity(Object[] elementData, int minCapacity) {
        // 添加元素时, 如果我们数组是为空走if语句(第一次)
        if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
            return Math.max(DEFAULT_CAPACITY, minCapacity);
        }
        return minCapacity;
    }

    // modCount++ ; 增删改查的时候 modCount++
    private void ensureExplicitCapacity(int minCapacity) {
        // 相当于判断我们数组中是否需要继续扩容
        if (minCapacity - elementData.length > 0) {
            // 对我们的数组实现扩容
            grow(minCapacity);
        }
    }

    private void grow(int minCapacity) {
        // 获取原来的数组长度
        int oldCapacity = elementData.length;
        // 获取新的数组长度    oldCapacity >> 1 表示 oldCapacity/2
        int newCapacity = oldCapacity + (oldCapacity >> 1);
        // 第一次执行
        if (newCapacity - minCapacity < 0){
            // 默认长度为10
            newCapacity = minCapacity;
        }
        // 如果我们的扩容长度大于Integer 最大值的情况下
        // 限制我们数组扩容最大值
        if (newCapacity - MAX_ARRAY_SIZE > 0){
            newCapacity = hugeCapacity(minCapacity);
        }
        // 对我们的数组进行扩容  将旧的数组数据复制到新的数组中
        elementData = Arrays.copyOf(elementData, newCapacity);
    }


    /**
     *  判断我们最小的初始化容量
     * @param minCapacity 最小容量>Integer 21 最大值的情况下  Integer.MAX_VALUE
     * @return
     */
    private static int hugeCapacity(int minCapacity) {
        if (minCapacity < 0) // overflow
            throw new OutOfMemoryError();
        return (minCapacity > MAX_ARRAY_SIZE) ?
                Integer.MAX_VALUE :
                MAX_ARRAY_SIZE;
    }

}
