#include <string>
#include <fstream>

#include <map>
#include <vector>

#include "ConfigFileParser.h"
#include "FileStream.h"
using namespace std;

void main()
{
	ConfigFileParser cConfig("ConstructSentence-config.ini", "=");

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
	vector<string> vecSentence;
	while(true)
	{
		CReadFile.GetOneCRFSentence(structSentence);
		if(structSentence.mapWords.size() == 0)
			break;
		mapCRFWords.clear();
		mapCRFWords = structSentence.mapWords;
	
		string strChuck;
		string strChuckTemp;
		vector<string> ::iterator itervecCRF;
		vecSentence.clear();

		for(int i=0;i < mapCRFWords.size();i++)
		{
			itervecCRF = mapCRFWords[i].begin();
			strChuckTemp = *(++itervecCRF);
			
			if(mapCRFWords[i].back() == "S")
				vecSentence.push_back(strChuckTemp);
			else if(mapCRFWords[i].back() == "B")
				strChuck += strChuckTemp;
			else if(mapCRFWords[i].back() == "B1")
				strChuck += strChuckTemp;
			else if(mapCRFWords[i].back() == "B2")
				strChuck += strChuckTemp;
			else if(mapCRFWords[i].back() == "M")
				strChuck += strChuckTemp;
			else if(mapCRFWords[i].back() == "E")
			{
				strChuck += strChuckTemp;
				vecSentence.push_back(strChuck);
				strChuck = "";
			}

		}

		//输出分词结果
		itervecCRF = vecSentence.begin();
		int nNum = 0;
		fResult << structSentence.strSentenceNum << "	";
		for(; itervecCRF != vecSentence.end(); itervecCRF++)
		{
			fResult << nNum << "^" << *itervecCRF << " " ;
			nNum++;
		}
		fResult << endl;
	}
	
	fResult.close();	

}
	
