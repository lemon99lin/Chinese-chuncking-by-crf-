@echo off

copy data\Test1.pos
copy data\Ori1.pos

echo step1 构建CRF训练数据
copy release\SentenceToCRF\SentenceToCRF.exe
copy release\SentenceToCRF\SentenceToCRF-config.ini
SentenceToCRF.exe
del SentenceToCRF.exe
del SentenceToCRF-config.ini

echo step2 测试
crf_test -m model1-f2-c4 CRF1.pos -o CRFRec1.pos

echo step3 重组句子
copy release\ConstructSentence\ConstructSentence.exe
copy release\ConstructSentence\ConstructSentence-config.ini
ConstructSentence.exe
del ConstructSentence.exe
del ConstructSentence-config.ini

echo step4 评价结果
copy release\JudgeSentence\JudgeSentence.exe
copy release\JudgeSentence\JudgeSentence-config.ini
JudgeSentence.exe
del JudgeSentence.exe
del JudgeSentence-config.ini
