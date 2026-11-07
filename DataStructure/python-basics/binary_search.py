def binary_search(sorted_arr, target):
    left, right = 0, len(sorted_arr) - 1
    
    while left <= right:
        mid = (left + right) // 2
        if sorted_arr[mid] == target:
            return mid
        elif sorted_arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return -1

# 测试数据 (必须是已排序的)
test_data = [12, 45, 67, 89, 34, 23, 90, 11]
sorted_test_data = sorted(test_data)  # 对测试数据排序
print(f"有序数组：{sorted_test_data}")
target_number = 34

# 执行查找
result = binary_search(sorted_test_data, target_number)

# 输出结果
if result != -1:
    print(f"目标数字 {target_number} 在有序数组中的索引是：{result}")
else:
    print(f"在有序数组中未找到目标数字 {target_number}")