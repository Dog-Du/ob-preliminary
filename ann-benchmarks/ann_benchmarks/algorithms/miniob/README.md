# 在 MiniOB 上运行 ann-benchmarks

0. 下载ann-benchmarks 源码安装所需 python 依赖
```
git clone https://github.com/nautaa/ann-benchmarks.git -b miniob_ann
cd ann-benchmarks
pip install -r requirements.txt
```
1. 以 mysql 通讯协议且监听 unix socket 的方式启动 miniob.
```bash
# 示例命令
/root/miniob/build_release/bin/observer -s /tmp/miniob.sock -P mysql 
```
2. 运行 ann-benchmark.
注意：需要将 `algorithms/miniob/config.yml` 中的 `arg_groups: [{unix_socket: "/tmp/miniob.sock"}]` 修改为 miniob 实际使用的 unix socket 文件地址
```bash
# 示例命令
python3 run.py --dataset fashion-mnist-784-euclidean --docker-tag ann-benchmarks-miniob --local --timeout 100 --runs 1
```
3. 生成运行结果.
```bash
# 示例命令
python3 plot.py --dataset fashion-mnist-784-euclidean
# 示例输出如下，其中每行结果倒数第一个值为该算法对应的QPS，每行结果倒数第二个值为该算法对应的召回率。
writing output to results/fashion-mnist-784-euclidean.png
Computing knn metrics
  0:                                                     MiniOBVector(query_probes=1)        0.968      167.476
Computing knn metrics
  1:                                                                 BruteForceBLAS()        1.000      355.359
```
