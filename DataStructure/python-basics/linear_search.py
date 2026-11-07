def linear_search(arr, target):
    for i in range(len(arr)):
        if arr[i] == target:
            return i
    return -1
    
# 测试数据
test_data = [12, 45, 67, 89, 34, 23, 90, 11]
target_number = 34

# 执行查找
result = linear_search(test_data, target_number)

# 输出结果
if result != -1:
    print(f"目标数字 {target_number} 在数组中的索引是：{result}")
else:
    print(f"在数组中未找到目标数字 {target_number}")