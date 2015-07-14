#include <string>
#include <fstream>

#include <map>
#include <vector>

#include "ConfigFileParser.h"
#include "FileStream.h"

using namespace std;//使用string必须加using namespace std

void main()
{
	ConfigFileParser cConfig("PosToCRF-config.ini", "=");

	string strPosPath = cConfig.GetValueAsString("IN_Pos_FILE");
	string strResultPath = cConfig.GetValueAsString("OUT_Result_FILE");
	string strSentenceNum = cConfig.GetValueAsString("OUT_SentenceNum_FILE");

//read and open files
	fstream fPos(strPosPath, ios_base::in);
	if (!fPos.is_open())
	{
		cout << "Pos File Error" << endl;
		system("pause");
		return ;
	}
	fstream fSentenceNum(strSentenceNum, ios_base::out);
	fstream fPosCRF(strResultPath, ios_base::out);
	
//读入词法文件内容
		string strTemp;
		map<string, vector<string>> mapPos;
		vector<string> vecTemp;
		while (getline(fPos, strTemp))
		{
			mapPos.clear();
			vecTemp.clear();
			int nTab = strTemp.find("	");
			string strNumber = strTemp.substr(0, nTab);
			strTemp = strTemp.substr(nTab+1) + " ";
			StringTokenizer cToken(strTemp, " ", false);
			while (cToken.HasMoreTokens())
			{
				vecTemp.push_back(cToken.NextToken());
			}

			//保存该句词法
			mapPos[strNumber] = vecTemp;
			//写入句子标号
			fSentenceNum << strNumber << endl;
			map<int, vector<string>> mapChar;
			vector<string> vecChar;
			vector<string>::iterator itevecPos;
			for (itevecPos = vecTemp.begin(); itevecPos != vecTemp.end(); itevecPos++)
			{
			//分解词法
				mapChar.clear();
				string strPos;
				string strPosOri = *itevecPos;
				int nOrder = strPosOri.find("^");
				int nCiXing = strPosOri.rfind("/");
				string strCiXing = strPosOri.substr(nCiXing+1);
			//找到词的内容
				strPos = strPosOri.substr(nOrder+1, nCiXing-nOrder-1);
				
			//分解词法
				int i = 0;
				int iPosition = 0;

				string strChar;
				while(i < strPos.length())
				{
					if(strPos[i] >= 0)//english or number
					{
						
						
							strChar = strPos[i];
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
						if(strCiXing == "w")
						{
							vecChar.push_back("P");
							i = strPos.length();
							strChar = strPos;
						}
						else
						{
							
							if(-80 < strPos[i+1]&& strPos[i+1] < -71)//全角数字
							{
							int n = 1;
							while( i+n < strPos.length()&& strPos[i+n] > -81 &&strPos[i+n] < -70 )
								n += 2;
							strChar = strPos.substr(i, n-1);
							vecChar.push_back("N");
							i += n-1;
							}
							else
							{
							strChar = strPos.substr(i,2);
							vecChar.push_back("C");
							i += 2 ;
							}
						}
						vecChar.push_back(strChar);
						mapChar[iPosition] = vecChar;
						vecChar.clear();
						iPosition++;
					}
				}
				if(iPosition == 1)
					mapChar[0].push_back("S");
				else if(iPosition == 2)
				{
					mapChar[0].push_back("B");
					mapChar[1].push_back("E");
				}
				else if(iPosition == 3)
				{
					mapChar[0].push_back("B");
					mapChar[1].push_back("B1");
					mapChar[2].push_back("E");
				}
				else if(iPosition == 4)
				{
					mapChar[0].push_back("B");
					mapChar[1].push_back("B1");
					mapChar[2].push_back("B2");
					mapChar[3].push_back("E");
				}
				else
				{
					mapChar[0].push_back("B");
					mapChar[1].push_back("B1");
					mapChar[2].push_back("B2");
					for(i = 3; i < iPosition - 1; i++)
						mapChar[i].push_back("M");
					mapChar[iPosition-1].push_back("E");	
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
			}
			fPosCRF << endl;
				
		}


		fPos.close();
		fSentenceNum.close();
		fPosCRF.close();

}
	
