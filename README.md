# streaminglib

### 用例

#### ams_sketch

```python
test_size = 1000
seeds = [2, 3, 5, 7]
# 把 sketch 大小修改到与 testset 相适应的大小
# 4 个hash函数, 每一个有256个cell
sketch = bf.ams_sketch(4, 256, seeds)
```

下面三行分别是
- 插入元素的数组, 和它们的频率数组
- 查询各个元素的频率
- 频率数组的模的平方

```python
sketch.insert_element(id_list, freqs)

result = sketch.obj_cou nt(id_list)

result_f2 = sketch.get_estimateF2()
```

#### spacesaving

构造函数需要传入堆的大小

插入一个元素也可以插入它对应的频率

查询top_k返回一个数组

```python
sketch = bf.spacesaving(8)

sketch.insert(ele, freq = 1)

res = sketch.query_top()
```

在均匀分布的数据中, 它的表现并不是很好, 在有偏的数据集上取得了比之前好的效果

#### skim_sketch

需要两个频率的数组, sketch构造时计算出对这两个数组内积(join size)的估计

构造时分别传入两个频率数组, 和domain size(数组的大小)

构造后, 访问join_size变量就可以得到对内积的估计

```python
sketch = bf.skim_sketch(freq1, freq2, domain_size)

res = sketch.join_size
```

在本地的测试中(自动生成的均匀分布的数据), 我们得到了大致5%-20%的错误率, 和原论文中<7%的错误率相比, 并不很差.

###### python接口或有问题, 以上的测试结果在c++中得到
