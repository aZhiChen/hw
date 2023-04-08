1、要先跑writeEgv.py文件，求出图的拉普拉斯矩阵特征分解后的特征向量，并将其记录在相应的文件目录中。运行命令为：python writeEgv.py --dataset_path ./data

2、引用一下[GLNN论文代码](https://github.com/snap-research/graphless-neural-networks)的数据集使用：

### Preparing datasets

To run experiments for dataset used in the paper, please download from the following links and put them under `data/` (see below for instructions on organizing the datasets).

- *CPF data* (`cora`, `citeseer`, `pubmed`, `a-computer`, and `a-photo`): Download the '.npz' files from [here](https://github.com/BUPT-GAMMA/CPF/tree/master/data/npz). Rename `amazon_electronics_computers.npz` and `amazon_electronics_photo.npz` to `a-computer.npz` and `a-photo.npz` respectively.

- *OGB data* (`ogbn-arxiv` and `ogbn-products`): Datasets will be automatically downloaded when running the `load_data` function in `dataloader.py`. More details [here](https://ogb.stanford.edu/).

- *BGNN data* (`house_class` and `vk_class`): Follow the instructions [here](https://github.com/dmlc/dgl/tree/473d5e0a4c4e4735f1c9dc9d783e0374328cca9a/examples/pytorch/bgnn) and download dataset pre-processed in DGL format from [here](https://www.dropbox.com/s/verx1evkykzli88/datasets.zip).

- *NonHom data* (`penn94` and `pokec`): Follow the instructions [here](https://github.com/CUAI/Non-Homophily-Benchmarks) to download the `penn94` dataset and its splits. The `pokec` dataset will be automatically downloaded when running the `load_data` function in `dataloader.py`.

- Your favourite datasets: download and add to the `load_data` function in `dataloader.py`.


3、先训练教师程序：python train_teacher.py --exp_setting ind --num_exp 5 --max_epoch 200 --patience 50

4、训练学生模型：python train_student.py --exp_setting ind --out_t_path outputs --num_exp 5 --max_epoch 200 --patience 50 --lamb 5 --distill hidden --alpha 1e-10

注释：

GNN模型和数据集的选择可在train_teacher.py或train_student.py文件中的main函数中进行修改。
