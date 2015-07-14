#include <string>
#include <fstream>

#include <map>
#include <vector>

#include "ConfigFileParser.h"
#include "FileStream.h"
using namespace std;

void main()
{
	ConfigFileParser cConfig("JudgeResult-config.ini", "=");

	string strPosPath = cConfig.GetValueAsString("IN_CRFPos_FILE");
	string strSentenceNum = cConfig.GetValueAsString("IN_SentenceNum_FILE");
	string strResultPath = cConfig.GetValueAsString("OUT_Result_FILE");
	
	CFileStream<string, string> CReadFile;
	CReadFile.ReadFileDataCRF(strSentenceNum, strPosPath);
	fstream fResult(strResultPath, ios_base::out);

	//读入句子编号及CRF分词内容
	struct g_struct_CRFData structSentence;
	map<int, vector<string>> mapCRFWords;
	vector<string>::iterator itevecCRF;
	double iAllWord = 0; 
	double iAllCorrectRec = 0;
	double iAllRec = 0;
	double doAllReCall = 0;
	double doAllPre = 0;
	double doAllF1 = 0;
	while(true)
	{
		CReadFile.GetOneCRFSentence(structSentence);
		if(structSentence.mapWords.size() == 0)
			break;
		mapCRFWords.clear();
		mapCRFWords = structSentence.mapWords;
		double doReCall = 0;
		double doPre = 0;
		double doF1 = 0;
		double iTotalWord = 0;
		double iCorrectRec = 0;
		double iRec = 0;
		for(int i=0;i < mapCRFWords.size();i++)
		{
			if(mapCRFWords[i].size() == 4)
			{
				string strRecResult;
				string strCorResult;
				strRecResult = mapCRFWords[i].back();
				itevecCRF = mapCRFWords[i].end();
				itevecCRF--;
				itevecCRF--;
				strCorResult = *(itevecCRF);
				if(strRecResult == strCorResult)
					iCorrectRec++;
				iRec++;
			}
			iTotalWord++;	
		}
		doReCall = iCorrectRec/iTotalWord;
		doPre = iCorrectRec/iRec;
		doF1 = 2*doReCall*doPre/(doReCall+doPre);
		fResult << structSentence.strSentenceNum << "	" << "Recall:" << doReCall << "	Precision:" <<  doPre << "	F1:" << doF1 << endl;
		iAllWord += iTotalWord;
		iAllCorrectRec += iCorrectRec;
		iAllRec += iRec;	
	}
	doAllReCall = iAllCorrectRec/iAllWord;
	doAllPre = iAllCorrectRec/iAllRec;
	doAllF1 = 2*doAllReCall*doAllPre/(doAllReCall+doAllPre);
	fResult << "平均值"<< "	" << "Recall:" << doAllReCall << "	Precision:" <<  doAllPre << "	F1:" << doAllF1 << endl;
	
	fResult.close();
//CReadFile.m_fCRFSentenceNum.close();
//	CReadFile.m_fCRFSentence.close();
			

}
	



