#pragma once

#define SAFE_RELEASE(x) {if(x) { (x)->Release(); (x) = NULL;} }

#include <Windows.h>
#include<DirectXMath.h>
#include<d3d11.h>
#include<d3dcompiler.h>
#include <tchar.h>
#include<iostream>
#include<vector>

#pragma comment( lib, "d3d11.lib" )
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "windowscodecs.lib")