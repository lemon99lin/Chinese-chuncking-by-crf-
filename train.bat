@echo off
echo step1 训练数据分字
copy release\PosToCRF\PosToCRF.exe
copy release\PosToCRF\PosToCRF-config.ini
PosToCRF.exe
del PosToCRF.exe
del PosToCRF-config.ini

echo step2 训练模型1
crf_learn -f 2 -c 4 -p 2 template result.txt model
system ("pause")

