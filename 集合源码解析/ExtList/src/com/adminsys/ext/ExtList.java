package com.adminsys.ext;

/**
 * @Author: qiang
 * @Description: List集合
 * @Create: 2019-12-11 11-46
 **/

public interface ExtList<E> {

    /**
     *  长度
     * @return
     */
    int size();

    /**
     *  添加节点
     * @param e
     * @return
     */
    boolean add(E e);

    /**
     *  获取节点
     * @param index
     * @return
     */
    E get(int index);

}
