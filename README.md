# 🌀 🔥 Repositório do SFR3D 🔥 🌀

## Compilando

Para compilar o programa pela primeira vez em sua versão sem MPI basta fazer o seguinte procedimento:

```console
cp contrib/Makefile_base Makefile
make
```

O executaval será criado na pasta **bin**.


## 🌀 Exemplos

Na pasta [exemplos](https://github.com/lmn-labest/fvCell/tree/master/exemplos) temos alguns exemplos prontos. Para rodar o programa em sequencial basta ir na pasta bin e utilizar o seguinte comando:

```console
mvfCell input.dat output
```

### 🌀 Problema da cavidade quadrada

Descompactanto o exemplo e preparando o diretorio:

```console
cp exemplos/lid_driven_cavity.zip bin/
cd bin
unzip lid_driven_cavity.zip
mkdir lid_driven_cavity/output
mv mvfCell lid_driven_cavity
```

Rodando o exemplo

```console
mvfCell cavidadeQH2.dat output/cavidadeQH2
```
