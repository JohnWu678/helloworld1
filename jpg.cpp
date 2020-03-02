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
	//�½����ź��λͼ  
	Bitmap pDestBmp(lpRGBBi->biWidth, lpRGBBi->biHeight, linenum, pixelfor, lpRGBDib);
	//��JPEGͼƬ��ʽ�������ݵ�����
	GetEncoderClsid( L"image/jpeg", &pngClsid);   
	//���������Ӧ���ڴ���
	CreateStreamOnHGlobal(NULL,TRUE,&pStream);
	pDestBmp.Save(pStream, &pngClsid);        

	HGLOBAL hMem;    

	GetHGlobalFromStream(pStream, &hMem);
	//����ڴ���С
	iLength = GlobalSize(hMem); 

	if(0 == iLength)
	{
		CString strErr;
		strErr.Format("Return size is zero, errorcode equal %d\n", GetLastError());
		TRACE0((LPCSTR)strErr);

		*pJPGData = NULL;

		return iLength;
	}

	//����ڴ���׵�ַ
	char *pImgData = (char *)GlobalLock(hMem);
	//3.�����ڴ棬���ͨ�����磬�Ͱ�pImgData������ݷ��͹�ȥ��
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