#include <string>
#include <fstream>

#include <map>
#include <vector>

#include "ConfigFileParser.h"
#include "FileStream.h"

using namespace std;

void main()
{
	ConfigFileParser cConfig("SentenceToCRF-config.ini", "=");

	string strSenPath = cConfig.GetValueAsString("IN_Sentence_FILE");
	string strResultPath = cConfig.GetValueAsString("OUT_Result_FILE");
	string strSentenceNum = cConfig.GetValueAsString("OUT_SentenceNum_FILE");

//read and open files
	fstream fSen(strSenPath, ios_base::in);
	if (!fSen.is_open())
	{
		cout << "Sentence File Error" << endl;
		system("pause");
		return ;
	}
	fstream fSentenceNum(strSentenceNum, ios_base::out);
	fstream fPosCRF(strResultPath, ios_base::out);
	
//读入词法文件内容
		string strTemp;
		string strSentence;
		map<int, vector<string>> mapChar;
		vector<string> vecChar;

		while (getline(fSen, strTemp))
		{
			int nTab = strTemp.find("	");
			string strNumber = strTemp.substr(0, nTab);
			strSentence = strTemp.substr(nTab+1);
			
			//写入句子标号
			fSentenceNum << strNumber << endl;
			
			
			//分解词法
				
				mapChar.clear();
				vecChar.clear();

				int i = 0;
				int iPosition = 0;
				string strChar;
				while(i < strSentence.length())
				{
					if(strSentence[i] >= 0)//english or number
					{
	
							strChar = strSentence[i];
							vecChar.push_back("L");
							i++;				
						//保存分字数据
							vecChar.push_back(strChar);
							mapChar[iPosition] = vecChar;
							vecChar.clear();
							iPosition++;
					}
					else//character or annotation
					{
						if(strSentence[i] == -93 || strSentence[i] == -95)
						{
							if(-80 < strSentence[i+1]&& strSentence[i+1] < -70)//全角数字
							{
								int n = 1;
								while( i+n < strSentence.length()&& strSentence[i+n] > -81 &&strSentence[i+n] < -70 )
									n += 2;
								strChar = strSentence.substr(i, n-1);
								vecChar.push_back("N");
								i += n-1;
							}
							else
							{
							vecChar.push_back("P");
							strChar = strSentence.substr(i,2);
							i += 2 ;
							}
						}

						else
						{
							strChar = strSentence.substr(i,2);
							vecChar.push_back("C");
							i += 2 ;
						}
							vecChar.push_back(strChar);
							mapChar[iPosition] = vecChar;
							vecChar.clear();
							iPosition++;

					}
				}
				
				for(i = 0; i < iPosition; i++)
				{
					vector<string>::iterator itevecCRF;
					itevecCRF = mapChar[i].begin();
					for(;itevecCRF != mapChar[i].end(); itevecCRF++)
					{
						string temp =  *(itevecCRF);
						fPosCRF <<temp << "	" ;
					}
					fPosCRF << endl;
				}
				fPosCRF << endl;
			}
		fSen.close();
		fSentenceNum.close();
		fPosCRF.close();
				
}


	
