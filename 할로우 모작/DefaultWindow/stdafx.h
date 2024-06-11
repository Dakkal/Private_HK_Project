// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include <ctime>
#include <cmath>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>

#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Template.h"

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

// ���̺귯�� : ���̳ʸ��� ������ �Լ����� ��Ƴ��� ����
// �ڵ� ������ ���� ��â�� ��� �� �ϳ��� �ٸ� ���α׷����� ����� �� �ֵ��� �ü���� ����Ʈ���� �����ڿ� ���� ����
// ���̺귯���� ��ټ� �Լ����� �Լ��� ���� �� ������� �ʿ�� �ϱ� ������ ��ũ �Ǳ� ��, ��ó���⿡ ���� �ش� ��������� ���� ���α׷��� ���Խ��Ѿ� �Ѵ�.
// ���̺귯������ ������� ���α׷��� ��ũ�Ǿ� ������ ������ ������ ���α׷��� �̷�� �ȴ�.


#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")

#include <io.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

using namespace std;