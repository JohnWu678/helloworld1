int exRGBtoJPG(LPBITMAPINFOHEADER lpRGBBi, LPBYTE lpRGBDib, char** pJPGData)
{    
	IStream *pStream = NULL;
	CLSID pngClsid;
	int pixelfor = 0;
	int linenum = 0;
	int iLength;

	if(lpRGBBi->biBitCount == 24)
	{  
		linenum = lpRGBBi->biWidth*3;
		pixelfor = PixelFormat24bppRGB; 
	}
	else if(lpRGBBi->biBitCount == 8)
	{       
		linenum = lpRGBBi->biWidth;
		pixelfor = PixelFormat8bppIndexed;
	}        
	//新建缩放后的位图  
	Bitmap pDestBmp(lpRGBBi->biWidth, lpRGBBi->biHeight, linenum, pixelfor, lpRGBDib);
	//以JPEG图片格式储存数据到流中
	GetEncoderClsid( L"image/jpeg", &pngClsid);   
	//获得与流对应的内存句柄
	CreateStreamOnHGlobal(NULL,TRUE,&pStream);
	pDestBmp.Save(pStream, &pngClsid);        

	HGLOBAL hMem;    

	GetHGlobalFromStream(pStream, &hMem);
	//获得内存块大小
	iLength = GlobalSize(hMem); 

	if(0 == iLength)
	{
		CString strErr;
		strErr.Format("Return size is zero, errorcode equal %d\n", GetLastError());
		TRACE0((LPCSTR)strErr);

		*pJPGData = NULL;

		return iLength;
	}

	//获得内存块首地址
	char *pImgData = (char *)GlobalLock(hMem);
	//3.复制内存，如果通过网络，就把pImgData里的数据发送过去。
	*pJPGData = (char*)GlobalAlloc(GPTR, iLength);

	if(NULL == (*pJPGData))
	{
		TRACE0("Global allocate memory failed\n");

		return iLength;
	}

	memcpy(pJPGData, pImgData, iLength);

	::GlobalUnlock(hMem);

	pStream->Release();
	pStream = NULL;
}