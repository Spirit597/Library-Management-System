[TOC]



# Library-Management-System

A  simple Library-Management-System demo
图书管理员可以查看所有分区的书架信息，

- 有多少书架（滚动区域显示，初步选择，已确定）

  - ![捕获.PNG](https://i.loli.net/2019/10/20/FIblgMO4SzXA6aG.png)

- 现在的实现方式（栅格式可滑动窗口）,由于按钮大小和位置会随着按钮数量不同而动态变化，后来弃置了

- 每个书架上有多少书，有哪些书，（滚动显示）

  |                |                                          |      |
  | -------------- | ---------------------------------------- | ---- |
  | Shelf Number   | 书架的唯一标识                           |      |
  | Shelf Type     | 书架存放哪一类书（一个书架仅存放一类书） |      |
  | shelf Capacity | 剩余容量（小于一个特定的值，暂定max=30） |      |

  

### 功能描述与设定

- 单纯书架的改动

  - 划分功能：若某一分区（依据图书的类别划分）的书架闲置，图书馆管理员可将其划分至另一分区；或者删除书架（前提必须是该书架空闲）
  - 功能模块放在那里？放在每个书架详细信息的窗口中
  - 如何实现？编辑书架属性即可；
    - **划分**：将当前类别改为要划分的类别
    - **删除**：删除该书架的记录（此处又涉及到：要删除书架？那么编号就不连续了）
  - 购置书架：购置书架时，数据库中的书架编号怎么办？怎么分配呢？如果要删除书架？那么编号就不连续了。
    - **新增书架**：书架编号、书架Type、书架容量（默认初始置为30）
      - 暂定解决办法：autoincrement,书架编号自增。缺陷：删除时，中间会出现空缺，造成有些编号删除后就不会再使用了

- 涉及到书的增添、书的移动

  - **移动书的位置**：编辑书的属性时，移动书的所在位置

  -  移动时实质上是对Book表中bookshelf属性的更新，可以用<u>触发器</u>来实现：每次更新（自然是移动到另一个书架）时，触发器捕捉更新前的shelf number ，并在Book Shelf中相应的书架容量+1；

  - 类似的，再设置一个触发器，捕捉更新后的   shelf number ，并在Book Shelf中相应的书架容量-1

    ```sqlite
    CREATE TRIGGER [ADD_BOOK_TO] AFTER UPDATE OF [bookShelf] ON [Book] FOR EACH ROW
    BEGIN
        UPDATE
            [BookShelf]
        SET
            [ShelfCapacity] = [ShelfCapacity] - 1
        WHERE
            [ShelfNumber] = [NEW].[bookShelf];
    END;
    
    CREATE TRIGGER [move_book_from] BEFORE UPDATE OF [bookShelf] ON [Book] FOR EACH ROW
    BEGIN
        UPDATE
            [BookShelf]
        SET
            [ShelfCapacity] = [ShelfCapacity] + 1
        WHERE
            [ShelfNUmber] = [OLD].[bookShelf];
    END;
    ```

  - **修改书架容量**：

    - 购入书时，放置书的位置，减少书架容量；
    
    - 删除书时，增加所在书架的容量；
    
      ------

### 具体实现

- [ ] **BookShelfListWidget**:总的书架列表，单独开一个窗口：**新增书架** 按钮放在此窗口中；

  - [ ] 新增时，仅指定type，容量默认初始值；书架编号的话怎么分配呢？

  - [ ] 暂定解决办法：autoincrement,书架编号自增。缺陷：删除时，中间会出现空缺，造成有些编号删除后就不会再使用了；

  - [ ] 新建时只需指定Type

    ```sqlite
    //示例：
    insert into BookShelf (ShelfType)VALUES('B')
    ```

    

- [ ] **BookShelfDetailWidget**:单个书架详情页上 ：**划分、删除**

  - [ ] 呈现方式？
  
- [x] **BookDetailWidget**:编辑书的属性时，**移动书的所在位置**

  - [x] 上方：书的属性，LineEdit输入框；下方：调用原生呈现数据库表格的widget，用来显示书架列表；
    - [x] 单纯移动书的位置？指定新的bookShelf编号；至于Bookshelf表中容量的增减，已经利用触发器实现了
    - [x] 如果要更改书的类别？使用下拉菜单显示，change为信号，每触发一次更新一次书架列表（依据change后书的类别）

