class ListNode:
    def __init__(self, data):
        self.data = data
        self.next = None
        
class LinkedList:
    def __init__(self):
        self.head = None
        
    def append(self, data):
        new_node = ListNode(data)
        if not self.head:
            self.head = new_node
            return
        last_node = self.head
        while last_node.next:
            last_node = last_node.next
        last_node.next = new_node
        
    def prepend(self, data):
        """在链表头部添加节点"""
        new_node = ListNode(data)
        new_node.next = self.head
        self.head = new_node

    def delete(self, key):
        """删除第一个值为key的节点"""
        current_node = self.head

        # 如果要删除的是头节点
        if current_node and current_node.data == key:
            self.head = current_node.next
            current_node = None
            return

        prev_node = None
        # 遍历寻找要删除的节点
        while current_node and current_node.data != key:
            prev_node = current_node
            current_node = current_node.next

        if current_node is None:  # 没找到
            return

        # 找到后，绕过该节点进行连接
        prev_node.next = current_node.next
        current_node = None

    def print_list(self):
        """打印链表所有元素"""
        current_node = self.head
        while current_node:
            print(current_node.data, end=" -> ")
            current_node = current_node.next
        print("NULL")

# 测试链表
print("=== 单向链表操作演示 ===")
train = LinkedList()
train.append("车厢1-煤炭")
train.append("车厢2-木材")
train.prepend("车头")  # 在头部添加
train.append("车厢3-钢材")
print("初始列车：")
train.print_list()  # 输出：车头 -> 车厢1-煤炭 -> 车厢2-木材 -> 车厢3-钢材 -> NULL

train.delete("车厢2-木材")
print("卸下木材后：")
train.print_list()  # 输出：车头 -> 车厢1-煤炭 -> 车厢3-钢材 -> NULL