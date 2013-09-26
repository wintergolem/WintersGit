#include "FBXLoader.h"
#include "fbxsdk.h"

namespace AIE
{
#define	TEMP_DBG_STRING_SIZE	2048
	char	g_TempDebugString[TEMP_DBG_STRING_SIZE];

	void AIEPrintf(const char* s, ...)
	{	
		va_list aArgList;

		va_start(aArgList, s);

		_vsnprintf(g_TempDebugString,TEMP_DBG_STRING_SIZE,s,aArgList);
		OutputDebugStringA(g_TempDebugString);

		va_end(aArgList);
	}
	void DisplayContent(FbxNode* pNode);
	void DisplayTarget(FbxNode* pNode);
	void DisplayTransformPropagation(FbxNode* pNode);
	void DisplayGeometricTransform(FbxNode* pNode);
	void DisplayHierarchy(FbxNode* pNode, int pDepth);
	void DisplayMetaDataConnections(FbxObject* pNode);
	void DisplayString(const char* pHeader, const char* pValue  = "", const char* pSuffix  = "");
	void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix  = "");
	void DisplayInt(const char* pHeader, int pValue, const char* pSuffix  = "");
	void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix  = "");
	void Display2DVector(const char* pHeader, FbxVector2 pValue, const char* pSuffix  = "");
	void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix  = "");
	void DisplayColor(const char* pHeader, FbxColor pValue, const char* pSuffix  = "");
	void Display4DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix  = "");
	void DisplayCamera(FbxNode* pNode);
	void DisplayCamera(FbxCamera* pCamera, char* pName, FbxNode* pTargetNode = NULL, FbxNode* pTargetUpNode = NULL);
	void DisplayDefaultAnimationValues(FbxCamera* pCamera);
	void DisplayRenderOptions(FbxCamera* pCamera);
	void DisplayCameraViewOptions(FbxCamera* pCamera);
	void DisplayBackgroundProperties(FbxCamera* pCamera);
	void DisplayApertureAndFilmControls(FbxCamera* pCamera);
	void DisplayViewingAreaControls(FbxCamera* pCamera);
	void DisplayCameraPositionAndOrientation(FbxCamera* pCamera, FbxNode* pTargetNode, FbxNode* pUpTargetNode);
	void DisplayLight(FbxNode* pNode);
	void DisplayDefaultAnimation(FbxNode* pNode);
	void DisplayMaterial(FbxGeometry* pGeometry);
	void DisplayMesh(FbxNode* pNode);
	void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
	void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
	void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher);
	void DisplayCurveKeys(FbxAnimCurve* pCurve);
	void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);
	void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
	void DisplayTexture(FbxGeometry* pGeometry);
	void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode);
	void DisplayLink(FbxGeometry* pGeometry);
	void DisplayMarker(FbxNode* pNode);
	void DisplayShape(FbxGeometry* pGeometry);
	void DisplayGenericInfo(FbxScene* pScene);
	void DisplayUserProperties(FbxObject* pObject);
	void DisplayPivotsAndLimits(FbxNode* pNode);
	void DisplaySkeleton(FbxNode* pNode);	

	void DisplayContent(FbxScene* pScene)
	{
		int i;
		FbxNode* lNode = pScene->GetRootNode();

		if(lNode)
		{
			for(i = 0; i < lNode->GetChildCount(); i++)
			{
				DisplayContent(lNode->GetChild(i));
			}
		}
	}

	void DisplayContent(FbxNode* pNode)
	{
		FbxNodeAttribute::EType lAttributeType;
		int i;

		if(pNode->GetNodeAttribute() != NULL)
		{
			lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

			switch (lAttributeType)
			{
			case FbxNodeAttribute::eMarker:  
	//			DisplayMarker(pNode);
				break;

			case FbxNodeAttribute::eSkeleton:  
				DisplaySkeleton(pNode);
				break;

			case FbxNodeAttribute::eMesh:      
				DisplayMesh(pNode);
				break;

			case FbxNodeAttribute::eNurbs:      
			//	DisplayNurb(pNode);
				AIEPrintf("NURB\n\n");
				break;

			case FbxNodeAttribute::ePatch:     
			//	DisplayPatch(pNode);
				AIEPrintf("Patch\n\n");
				break;

			case FbxNodeAttribute::eCamera:    
	//			DisplayCamera(pNode);
				break;

			case FbxNodeAttribute::eLight:     
	//			DisplayLight(pNode);
				break;

			case FbxNodeAttribute::eLODGroup:
		//		DisplayLodGroup(pNode);
				AIEPrintf("LOD Group\n\n");
				break;
			}   
		}

//		DisplayUserProperties(pNode);
//		DisplayTarget(pNode);
//		DisplayPivotsAndLimits(pNode);
//		DisplayTransformPropagation(pNode);
//		DisplayGeometricTransform(pNode);

		for(i = 0; i < pNode->GetChildCount(); i++)
		{
			DisplayContent(pNode->GetChild(i));
		}
	}

	void DisplayTarget(FbxNode* pNode)
	{
		if(pNode->GetTarget() != NULL)
		{
			DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
		}
	}

	void DisplayTransformPropagation(FbxNode* pNode)
	{
		AIEPrintf("    Transformation Propagation\n");

		// 
		// Rotation Space
		//
		EFbxRotationOrder lRotationOrder;
		pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

		AIEPrintf("        Rotation Space: ");

		switch (lRotationOrder)
		{
		case eEulerXYZ: 
			AIEPrintf("Euler XYZ\n");
			break;
		case eEulerXZY:
			AIEPrintf("Euler XZY\n");
			break;
		case eEulerYZX:
			AIEPrintf("Euler YZX\n");
			break;
		case eEulerYXZ:
			AIEPrintf("Euler YXZ\n");
			break;
		case eEulerZXY:
			AIEPrintf("Euler ZXY\n");
			break;
		case eEulerZYX:
			AIEPrintf("Euler ZYX\n");
			break;
		case eSphericXYZ:
			AIEPrintf("Spheric XYZ\n");
			break;
		}

		//
		// Use the Rotation space only for the limits
		// (keep using eEulerXYZ for the rest)
		//
		AIEPrintf("        Use the Rotation Space for Limit specification only: %s\n",
			pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


		//
		// Inherit Type
		//
		FbxTransform::EInheritType lInheritType;
		pNode->GetTransformationInheritType(lInheritType);

		AIEPrintf("        Transformation Inheritance: ");

		switch (lInheritType)
		{
		case FbxTransform::eInheritRrSs:
			AIEPrintf("RrSs\n");
			break;
		case FbxTransform::eInheritRSrs:
			AIEPrintf("RSrs\n");
			break;
		case FbxTransform::eInheritRrs:
			AIEPrintf("Rrs\n");
			break;
		}
	}

	void DisplayGeometricTransform(FbxNode* pNode)
	{
		FbxVector4 lTmpVector;

		AIEPrintf("    Geometric Transformations\n");

		//
		// Translation
		//
		lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		AIEPrintf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		//
		// Rotation
		//
		lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		AIEPrintf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		//
		// Scaling
		//
		lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		AIEPrintf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
	}

	void DisplayMetaDataConnections(FbxObject* pObject)
	{
		int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
		if (nbMetaData > 0)
			DisplayString("    MetaData connections ");

		for (int i = 0; i < nbMetaData; i++)
		{
			FbxObjectMetaData* metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
			DisplayString("        Name: ", (char*)metaData->GetName());
		}
	}

	void DisplayString(const char* pHeader, const char* pValue /* = "" */, const char* pSuffix /* = "" */)
	{
		FbxString lString;

		lString = pHeader;
		lString += pValue;
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix /* = "" */)
	{
		FbxString lString;

		lString = pHeader;
		lString += pValue ? "true" : "false";
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void DisplayInt(const char* pHeader, int pValue, const char* pSuffix /* = "" */)
	{
		FbxString lString;

		lString = pHeader;
		lString += pValue;
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix /* = "" */)
	{
		FbxString lString;
		FbxString lFloatValue = (float) pValue;

		lFloatValue = pValue <= -HUGE_VAL ? "-INFINITY" : lFloatValue.Buffer();
		lFloatValue = pValue >=  HUGE_VAL ?  "INFINITY" : lFloatValue.Buffer();

		lString = pHeader;
		lString += lFloatValue;
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void Display2DVector(const char* pHeader, FbxVector2 pValue, const char* pSuffix  /* = "" */)
	{
		FbxString lString;
		FbxString lFloatValue1 = (float)pValue[0];
		FbxString lFloatValue2 = (float)pValue[1];

		lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
		lFloatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : lFloatValue1.Buffer();
		lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
		lFloatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : lFloatValue2.Buffer();

		lString = pHeader;
		lString += lFloatValue1;
		lString += ", ";
		lString += lFloatValue2;
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix /* = "" */)
	{
		FbxString lString;
		FbxString lFloatValue1 = (float)pValue[0];
		FbxString lFloatValue2 = (float)pValue[1];
		FbxString lFloatValue3 = (float)pValue[2];

		lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
		lFloatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : lFloatValue1.Buffer();
		lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
		lFloatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : lFloatValue2.Buffer();
		lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
		lFloatValue3 = pValue[2] >=  HUGE_VAL ?  "INFINITY" : lFloatValue3.Buffer();

		lString = pHeader;
		lString += lFloatValue1;
		lString += ", ";
		lString += lFloatValue2;
		lString += ", ";
		lString += lFloatValue3;
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void Display4DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix /* = "" */)
	{
		FbxString lString;
		FbxString lFloatValue1 = (float)pValue[0];
		FbxString lFloatValue2 = (float)pValue[1];
		FbxString lFloatValue3 = (float)pValue[2];
		FbxString lFloatValue4 = (float)pValue[3];

		lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
		lFloatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : lFloatValue1.Buffer();
		lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
		lFloatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : lFloatValue2.Buffer();
		lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
		lFloatValue3 = pValue[2] >=  HUGE_VAL ?  "INFINITY" : lFloatValue3.Buffer();
		lFloatValue4 = pValue[3] <= -HUGE_VAL ? "-INFINITY" : lFloatValue4.Buffer();
		lFloatValue4 = pValue[3] >=  HUGE_VAL ?  "INFINITY" : lFloatValue4.Buffer();

		lString = pHeader;
		lString += lFloatValue1;
		lString += ", ";
		lString += lFloatValue2;
		lString += ", ";
		lString += lFloatValue3;
		lString += ", ";
		lString += lFloatValue4;
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void DisplayColor(const char* pHeader, FbxPropertyT<FbxDouble3> pValue, const char* pSuffix /* = "" */)

	{
		FbxString lString;

		lString = pHeader;
		//lString += (float) pValue.mRed;
		//lString += (double)pValue.GetArrayItem(0);
		lString += " (red), ";
		//lString += (float) pValue.mGreen;
		//lString += (double)pValue.GetArrayItem(1);
		lString += " (green), ";
		//lString += (float) pValue.mBlue;
		//lString += (double)pValue.GetArrayItem(2);
		lString += " (blue)";
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}
	void DisplayColor(const char* pHeader, FbxColor pValue, const char* pSuffix /* = "" */)
	{
		FbxString lString;

		lString = pHeader;
		lString += (float) pValue.mRed;

		lString += " (red), ";
		lString += (float) pValue.mGreen;

		lString += " (green), ";
		lString += (float) pValue.mBlue;

		lString += " (blue)";
		lString += pSuffix;
		lString += "\n";
		AIEPrintf(lString);
	}

	void DisplayCamera(FbxNode* pNode)
	{
		DisplayCamera((FbxCamera*) pNode->GetNodeAttribute(), (char *) pNode->GetName(), pNode->GetTarget(), pNode->GetTargetUp());
	}

	void DisplayCamera(FbxCamera* pCamera, char* pName, FbxNode* pTargetNode, FbxNode* pTargetUpNode)
	{
		DisplayString("Camera Name: ", pName);
		if (!pCamera)
		{
			DisplayString("NOT FOUND");
			return;
		}
		DisplayMetaDataConnections(pCamera);

		DisplayCameraPositionAndOrientation(pCamera, pTargetNode, pTargetUpNode);

		char* lProjectionTypes[] = { "Perspective", "Orthogonal" };

		DisplayString("    Projection Type: ", lProjectionTypes[pCamera->ProjectionType.Get()]);

		DisplayViewingAreaControls(pCamera);

		// If camera projection type is set to FbxCamera::eOrthogonal, the 
		// aperture and film controls are not relevant.
		if (pCamera->ProjectionType.Get() != FbxCamera::eOrthogonal)
		{
			DisplayApertureAndFilmControls(pCamera);
		}

		DisplayBackgroundProperties(pCamera);
		DisplayCameraViewOptions(pCamera);
		DisplayRenderOptions(pCamera);
		DisplayDefaultAnimationValues(pCamera);
	}

	void DisplayCameraPositionAndOrientation(FbxCamera* pCamera, FbxNode* pTargetNode, FbxNode* pTargetUpNode)
	{
		DisplayString("    Camera Position and Orientation");
		Display3DVector("        Position: ", pCamera->Position.Get());

		if (pTargetNode)
		{
			DisplayString("        Camera Interest: ",(char *) pTargetNode->GetName());
		}
		else
		{
			Display3DVector("        Default Camera Interest Position: ", pCamera->InterestPosition.Get());
		}

		if (pTargetUpNode)
		{
			DisplayString("        Camera Up Target: ", (char *) pTargetUpNode->GetName());
		}
		else
		{
			Display3DVector("        Up Vector: ", pCamera->UpVector.Get());
		}

		DisplayDouble("        Roll: ", pCamera->Roll.Get());
	}

	void DisplayViewingAreaControls(FbxCamera* pCamera)
	{
		DisplayString("    Viewing Area Controls");

		char* lCameraFormat[] = { "Custom", "D1 NTSC", "NTSC", "PAL", "D1 PAL", 
			"HD", "640x480", "320x200", "320x240", "128x128",
			"Full Screen"  };

		DisplayString("        Format: ", lCameraFormat[pCamera->GetFormat()]);

		char* lAspectRatioModes[] = { "Window Size", "Fixed Ratio", "Fixed Resolution", 
			"Fixed Width", "Fixed Height" };

		DisplayString("        Aspect Ratio Mode: ", lAspectRatioModes[pCamera->GetAspectRatioMode()]);

		// If the ratio mode is eWINDOW_SIZE, both width and height values aren't relevant.
		if (pCamera->GetAspectRatioMode() != FbxCamera::eWindowSize)
		{
			DisplayDouble("        Aspect Width: ", pCamera->AspectWidth.Get());
			DisplayDouble("        Aspect Height: ", pCamera->AspectHeight.Get());
		}

		DisplayDouble("        Pixel Ratio: ", pCamera->PixelAspectRatio.Get());
		DisplayDouble("        Near Plane: ", pCamera->NearPlane.Get());
		DisplayDouble("        Far Plane: ", pCamera->FarPlane.Get());
		DisplayBool("        Mouse Lock: ", pCamera->LockMode.Get());
	}

	void DisplayApertureAndFilmControls(FbxCamera* pCamera)
	{
		DisplayString("    Aperture and Film Controls");

		char* lCameraApertureFormats[] = { "Custom", 
			"16mm Theatrical",
			"Super 16mm",
			"35mm Academy",
			"35mm TV Projection",
			"35mm Full Aperture",
			"35mm 1.85 Projection",
			"35mm Anamorphic",
			"70mm Projection",
			"VistaVision",
			"Dynavision",
			"Imax" };

		DisplayString("        Aperture Format: ", lCameraApertureFormats[pCamera->GetApertureFormat()]);

		char* lCameraApertureModes[] = { "Horizontal and Vertical", "Horizontal", "Vertical", "Focal Length" };

		DisplayString("        Aperture Mode: ", lCameraApertureModes[pCamera->GetApertureMode()]);

		DisplayDouble("        Aperture Width: ", pCamera->GetApertureWidth(), " inches");
		DisplayDouble("        Aperture Height: ", pCamera->GetApertureHeight(), " inches");
		DisplayDouble("        Squeeze Ratio: ", pCamera->GetSqueezeRatio());
		DisplayDouble("        Focal Length: ", pCamera->FocalLength.Get(), "mm");
		DisplayDouble("        Field of View: ", pCamera->FieldOfView.Get(), " degrees");
	}

	void DisplayBackgroundProperties(FbxCamera* pCamera)
	{
		DisplayString("    Background Properties");

		DisplayString("        Background File Name: \"", (char *) pCamera->GetBackgroundFileName(), "\"");

		char* lBackgroundDisplayModes[] = { "Disabled", "Always", "When Media" };

		DisplayString("        Background Display Mode: ", lBackgroundDisplayModes[pCamera->ViewFrustumBackPlaneMode.Get()]);

		DisplayBool("        Foreground Matte Threshold Enable: ", pCamera->ShowFrontplate.Get());

		// This option is only relevant if background drawing mode is set to eFOREGROUND or eBACKGROUND_AND_FOREGROUND.
		if (pCamera->ForegroundOpacity.Get())
		{
			DisplayDouble("        Foreground Matte Threshold: ", pCamera->BackgroundAlphaTreshold.Get());
		}

		FbxString lBackgroundPlacementOptions;

		if (pCamera->GetBackPlateFitImage())
		{
			lBackgroundPlacementOptions += " Fit,";
		}
		if (pCamera->GetBackPlateCenter())
		{
			lBackgroundPlacementOptions += " Center,";
		}
		if (pCamera->GetBackPlateKeepRatio())
		{
			lBackgroundPlacementOptions += " Keep Ratio,";
		}
		if (pCamera->GetBackPlateCrop())
		{
			lBackgroundPlacementOptions += " Crop,";
		}
		if (!lBackgroundPlacementOptions.IsEmpty())
		{
			FbxString lString =  lBackgroundPlacementOptions.Left(lBackgroundPlacementOptions.GetLen() - 1);
			DisplayString("        Background Placement Options: ",lString.Buffer());
		}

		DisplayDouble("        Background Distance: ", pCamera->BackPlaneDistance.Get());

		char* lCameraBackgroundDistanceModes[] = { "Relative to Interest", "Absolute from Camera" };

		DisplayString("        Background Distance Mode: ", lCameraBackgroundDistanceModes[pCamera->BackPlaneDistanceMode.Get()]);
	}

	void DisplayCameraViewOptions(FbxCamera* pCamera)
	{
		DisplayString("    Camera View Options");

		DisplayBool("        View Camera Interest: ", pCamera->ViewCameraToLookAt.Get());
		DisplayBool("        View Near Far Planes: ", pCamera->ViewFrustumNearFarPlane.Get());
		DisplayBool("        Show Grid: ", pCamera->ShowGrid.Get());
		DisplayBool("        Show Axis: ", pCamera->ShowAzimut.Get());
		DisplayBool("        Show Name: ", pCamera->ShowName.Get());
		DisplayBool("        Show Info on Moving: ", pCamera->ShowInfoOnMoving.Get());
		DisplayBool("        Show Time Code: ", pCamera->ShowTimeCode.Get());
		DisplayBool("        Display Safe Area: ", pCamera->DisplaySafeArea.Get());

		char* lSafeAreaStyles[] = { "Round", "Square" };
		FbxColor color;
		FbxDouble3 c;

		DisplayString("        Safe Area Style: ", lSafeAreaStyles[pCamera->SafeAreaDisplayStyle.Get()]);
		DisplayBool("        Show Audio: ", pCamera->ShowAudio.Get());

		c = pCamera->BackgroundColor.Get();
		color = FbxColor(c[0], c[1], c[2]);
		DisplayColor("        Background Color: ", color);

		c = pCamera->AudioColor.Get();
		color = FbxColor(c[0], c[1], c[2]);
		DisplayColor("        Audio Color: ", color);

		DisplayBool("        Use Frame Color: ", pCamera->UseFrameColor.Get());

		c = pCamera->FrameColor.Get();
		color = FbxColor(c[0], c[1], c[2]);
		DisplayColor("        Frame Color: ", color);
	}

	void DisplayRenderOptions(FbxCamera* pCamera)
	{
		DisplayString("    Render Options");

		char* lCameraRenderOptionsUsageTimes[] = { "Interactive", "At Render" };

		DisplayString("        Render Options Usage Time: ", lCameraRenderOptionsUsageTimes[pCamera->UseRealTimeDOFAndAA.Get()]);
		DisplayBool("        Use Antialiasing: ", pCamera->UseAntialiasing.Get());
		DisplayDouble("        Antialiasing Intensity: ", pCamera->AntialiasingIntensity.Get());

		char* lCameraAntialiasingMethods[] = { "Oversampling Antialiasing", "Hardware Antialiasing" };

		DisplayString("        Antialiasing Method: ", lCameraAntialiasingMethods[pCamera->AntialiasingMethod.Get()]);

		// This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
		if (pCamera->AntialiasingMethod.Get() == FbxCamera::eAAOversampling)
		{
			DisplayInt("        Number of Samples: ", pCamera->FrameSamplingCount.Get());
		}

		char* lCameraSamplingTypes[] = { "Uniform", "Stochastic" };

		DisplayString("        Sampling Type: ", lCameraSamplingTypes[pCamera->FrameSamplingType.Get()]);
		DisplayBool("        Use Accumulation Buffer: ", pCamera->UseAccumulationBuffer.Get());
		DisplayBool("        Use Depth of Field: ", pCamera->UseDepthOfField.Get());

		char* lCameraFocusDistanceSources[] = { "Camera Interest", "Specific Distance" };

		DisplayString("        Focus Distance Source: ", lCameraFocusDistanceSources[pCamera->FocusSource.Get()]);

		// This parameter is only relevant if focus distance source is set to eSPECIFIC_DISTANCE.
		if (pCamera->FocusSource.Get() == FbxCamera::eFocusSpecificDistance)
		{
			DisplayDouble("        Specific Distance: ", pCamera->FocusDistance.Get());
		}

		DisplayDouble("        Focus Angle: ", pCamera->FocusAngle.Get(), " degrees");
	}

	void DisplayDefaultAnimationValues(FbxCamera* pCamera)
	{
		DisplayString("    Default Animation Values");

		DisplayDouble("        Default Field of View: ", pCamera->FieldOfView.Get());
		DisplayDouble("        Default Field of View X: ", pCamera->FieldOfViewX.Get());
		DisplayDouble("        Default Field of View Y: ", pCamera->FieldOfViewY.Get());
		DisplayDouble("        Default Optical Center X: ", pCamera->OpticalCenterX.Get());
		DisplayDouble("        Default Optical Center Y: ", pCamera->OpticalCenterY.Get());
		DisplayDouble("        Default Roll: ", pCamera->Roll.Get());
	}

	void DisplayDefaultAnimationValues(FbxLight* pLight)
	{
		DisplayString("    Default Animation Values");

		FbxDouble3 c = pLight->Color.Get();
		FbxColor lColor(c[0], c[1], c[2]);
		DisplayColor("        Default Color: ", lColor);
		DisplayDouble("        Default Intensity: ", pLight->Intensity.Get());
		DisplayDouble("        Default Outer Angle: ", pLight->OuterAngle.Get());
		DisplayDouble("        Default Fog: ", pLight->Fog.Get());
	}

	void DisplayLight(FbxNode* pNode)
	{
		FbxLight* lLight = (FbxLight*) pNode->GetNodeAttribute();

		DisplayString("Light Name: ", (char *) pNode->GetName());
		DisplayMetaDataConnections(lLight);

		char* lLightTypes[] = { "Point", "Directional", "Spot" };

		DisplayString("    Type: ", lLightTypes[lLight->LightType.Get()]);
		DisplayBool("    Cast Light: ", lLight->CastLight.Get());

		if (!(lLight->FileName.Get().IsEmpty()))
		{
			DisplayString("    Gobo");

			DisplayString("        File Name: \"", lLight->FileName.Get().Buffer(), "\"");
			DisplayBool("        Ground Projection: ", lLight->DrawGroundProjection.Get());
			DisplayBool("        Volumetric Projection: ", lLight->DrawVolumetricLight.Get());
			DisplayBool("        Front Volumetric Projection: ", lLight->DrawFrontFacingVolumetricLight.Get());
		}

		DisplayDefaultAnimationValues(lLight);
	}

	void DisplayMaterial(FbxGeometry* pGeometry)
	{
		int lMaterialCount = 0;
		FbxNode* lNode = NULL;
		if(pGeometry){
			lNode = pGeometry->GetNode();
			if(lNode)
				lMaterialCount = lNode->GetMaterialCount();    
		}

		if (lMaterialCount > 0)
		{
			FbxPropertyT<FbxDouble3> lKFbxDouble3;
			FbxPropertyT<FbxDouble> lKFbxDouble1;
			FbxColor theColor;

			for (int lCount = 0; lCount < lMaterialCount; lCount ++)
			{
				DisplayInt("        Material ", lCount);

				FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(lCount);

				DisplayString("            Name: \"", (char *) lMaterial->GetName(), "\""); 

				//Get the implementation to see if it's a hardware shader.
				const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
				FbxString lImplemenationType = "HLSL";
				if(!lImplementation)
				{
					lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
					lImplemenationType = "CGFX";
				}
				if(lImplementation)
				{
					//Now we have a hardware shader, let's read it
					AIEPrintf("            Hardware Shader Type: %s\n", lImplemenationType.Buffer());
					FbxBindingTable const* lRootTable = lImplementation->GetRootTable();
					FbxString lFileName = lRootTable->DescAbsoluteURL.Get();
					FbxString lTechniqueName = lRootTable->DescTAG.Get(); 


					FbxBindingTable const* lTable = lImplementation->GetRootTable();
					size_t lEntryNum = lTable->GetEntryCount();

					for(int i=0;i <(int)lEntryNum; ++i)
					{
						const FbxBindingTableEntry& lEntry = lTable->GetEntry(i);
						const char* lEntrySrcType = lEntry.GetEntryType(true); 
						FbxProperty lFbxProp;


						FbxString lTest = lEntry.GetSource();
						AIEPrintf("            Entry: %s\n", lTest.Buffer());


						if ( strcmp( FbxPropertyEntryView::sEntryType, lEntrySrcType ) == 0 )
						{   
							lFbxProp = lMaterial->FindPropertyHierarchical(lEntry.GetSource()); 
							if(!lFbxProp.IsValid())
							{
								lFbxProp = lMaterial->RootProperty.FindHierarchical(lEntry.GetSource());
							}


						}
						else if( strcmp( FbxConstantEntryView::sEntryType, lEntrySrcType ) == 0 )
						{
							lFbxProp = lImplementation->GetConstants().FindHierarchical(lEntry.GetSource());
						}
						if(lFbxProp.IsValid())
						{
							if( lFbxProp.GetSrcObjectCount<FbxTexture>() > 0 )
							{
								//do what you want with the textures
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxFileTexture>(); ++j)
								{
									FbxFileTexture *lTex = lFbxProp.GetSrcObject<FbxFileTexture>(j);
									AIEPrintf("           File Texture: %s\n", lTex->GetFileName());
								}
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxLayeredTexture>(); ++j)
								{
									FbxLayeredTexture *lTex = lFbxProp.GetSrcObject<FbxLayeredTexture>(j);
									AIEPrintf("        Layered Texture: %s\n", lTex->GetName());
								}
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxProceduralTexture>(); ++j)
								{
									FbxProceduralTexture *lTex = lFbxProp.GetSrcObject<FbxProceduralTexture>(j);
									AIEPrintf("     Procedural Texture: %s\n", lTex->GetName());
								}
							}
							else
							{
								FbxDataType lFbxType = lFbxProp.GetPropertyDataType();
								FbxString blah = lFbxType.GetName();
								if(FbxBoolDT == lFbxType)
								{
									DisplayBool("                Bool: ", lFbxProp.Get<FbxBool>() );
								}
								else if ( FbxIntDT == lFbxType ||  FbxEnumDT  == lFbxType )
								{
									DisplayInt("                Int: ", lFbxProp.Get<FbxInt>());
								}
								else if ( FbxFloatDT == lFbxType)
								{
									DisplayDouble("                Float: ", lFbxProp.Get<FbxFloat>());

								}
								else if ( FbxDoubleDT == lFbxType)
								{
									DisplayDouble("                Double: ", lFbxProp.Get<FbxDouble>());
								}
								else if ( FbxStringDT == lFbxType
									||  FbxUrlDT  == lFbxType
									||  FbxXRefUrlDT  == lFbxType )
								{
									DisplayString("                String: ", lFbxProp.Get<FbxString>().Buffer());
								}
								else if ( FbxDouble2DT == lFbxType)
								{
									FbxDouble2 lDouble2 = lFbxProp.Get<FbxDouble2>();
									FbxVector2 lVect;
									lVect[0] = lDouble2[0];
									lVect[1] = lDouble2[1];

									Display2DVector("                2D vector: ", lVect);
								}
								else if ( FbxDouble3DT == lFbxType || FbxColor3DT == lFbxType)
								{
									FbxDouble3 lDouble3 = lFbxProp.Get<FbxDouble3>();


									FbxVector4 lVect;
									lVect[0] = lDouble3[0];
									lVect[1] = lDouble3[1];
									lVect[2] = lDouble3[2];
									Display3DVector("                3D vector: ", lVect);
								}

								else if ( FbxDouble4DT == lFbxType || FbxColor4DT == lFbxType)
								{
									FbxDouble4 lDouble4 = lFbxProp.Get<FbxDouble4>();
									FbxVector4 lVect;
									lVect[0] = lDouble4[0];
									lVect[1] = lDouble4[1];
									lVect[2] = lDouble4[2];
									lVect[3] = lDouble4[3];
									Display4DVector("                4D vector: ", lVect);
								}
								else if ( FbxDouble4x4DT == lFbxType)
								{
									FbxDouble4x4 lDouble44 = lFbxProp.Get<FbxDouble4x4>();
									for(int j=0; j<4; ++j)
									{

										FbxVector4 lVect;
										lVect[0] = lDouble44[j][0];
										lVect[1] = lDouble44[j][1];
										lVect[2] = lDouble44[j][2];
										lVect[3] = lDouble44[j][3];
										Display4DVector("                4x4D vector: ", lVect);
									}

								}
							}

						}   
					}
				}
				else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
				{
					// We found a Phong material.  Display its properties.

					// Display the Ambient Color
					lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Ambient;
					theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
					DisplayColor("            Ambient: ", theColor);

					// Display the Diffuse Color
					lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Diffuse;
					theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
					DisplayColor("            Diffuse: ", theColor);

					// Display the Specular Color (unique to Phong materials)
					lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Specular;
					theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
					DisplayColor("            Specular: ", theColor);

					// Display the Emissive Color
					lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Emissive;
					theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
					DisplayColor("            Emissive: ", theColor);

					//Opacity is Transparency factor now
					lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->TransparencyFactor;
					DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());

					// Display the Shininess
					lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->Shininess;
					DisplayDouble("            Shininess: ", lKFbxDouble1.Get());

					// Display the Reflectivity
					lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->ReflectionFactor;
					DisplayDouble("            Reflectivity: ", lKFbxDouble1.Get());
				}
				else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
				{
					// We found a Lambert material. Display its properties.
					// Display the Ambient Color
					lKFbxDouble3=((FbxSurfaceLambert *)lMaterial)->Ambient;
					theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
					DisplayColor("            Ambient: ", theColor);

					// Display the Diffuse Color
					lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Diffuse;
					theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
					DisplayColor("            Diffuse: ", theColor);

					// Display the Emissive
					lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Emissive;
					theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
					DisplayColor("            Emissive: ", theColor);

					// Display the Opacity
					lKFbxDouble1 =((FbxSurfaceLambert *)lMaterial)->TransparencyFactor;
					DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());
				}
				else
					DisplayString("Unknown type of Material");

				FbxPropertyT<FbxString> lString;
				lString = lMaterial->ShadingModel;
				DisplayString("            Shading Model: ", lString.Get().Buffer());
				DisplayString("");
			}
		}
	}

#define MAT_HEADER_LENGTH 200

	void DisplayControlsPoints(FbxMesh* pMesh);
	void DisplayPolygons(FbxMesh* pMesh);
	void DisplayMaterialMapping(FbxMesh* pMesh);
	void DisplayTextureMapping(FbxMesh* pMesh);
	void DisplayTextureNames( FbxProperty &pProperty, FbxString& pConnectionString );
	void DisplayMaterialConnections(FbxMesh* pMesh);
	void DisplayMaterialTextureConnections( FbxSurfaceMaterial* pMaterial, 	char * header, int pMatId, int l );

	void DisplayMesh(FbxNode* pNode)
	{
		FbxMesh* lMesh = (FbxMesh*) pNode->GetNodeAttribute ();

		DisplayString("Mesh Name: ", (char *) pNode->GetName());
	//	DisplayMetaDataConnections(lMesh);
	//	DisplayControlsPoints(lMesh);
//		DisplayPolygons(lMesh);
	//	DisplayMaterialMapping(lMesh);
	//	DisplayMaterial(lMesh);
	//	DisplayTexture(lMesh);
	//	DisplayMaterialConnections(lMesh);
		DisplayLink(lMesh);
	//	DisplayShape(lMesh);
	}

	void DisplayControlsPoints(FbxMesh* pMesh)
	{
		int i, lControlPointsCount = pMesh->GetControlPointsCount();
		FbxVector4* lControlPoints = pMesh->GetControlPoints();

		DisplayString("    Control Points");

		for (i = 0; i < lControlPointsCount; i++)
		{
			DisplayInt("        Control Point ", i);
			Display3DVector("            Coordinates: ", lControlPoints[i]);

			for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
			{
				FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal( j);
				if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					char header[100];
					FBXSDK_sprintf(header, 100, "            Normal Vector: "); 
					if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
						Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
				}
			}
		}

		DisplayString("");
	}
	void DisplayPolygons(FbxMesh* pMesh)
	{
		int i, j, lPolygonCount = pMesh->GetPolygonCount();
		FbxVector4* lControlPoints = pMesh->GetControlPoints(); 
		char header[100];

		DisplayString("    Polygons");

		int vertexId = 0;
		for (i = 0; i < lPolygonCount; i++)
		{
			DisplayInt("        Polygon ", i);
			int l;

			for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
			{
				FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
				switch (lePolgrp->GetMappingMode())
				{
				case FbxGeometryElement::eByPolygon:
					if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
					{
						FBXSDK_sprintf(header, 100, "        Assigned to group: "); 
						int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
						DisplayInt(header, polyGroupId);
						break;
					}
				default:
					// any other mapping modes don't make sense
					DisplayString("        \"unsupported group assignment\"");
					break;
				}
			}

			int lPolygonSize = pMesh->GetPolygonSize(i);

			if (lPolygonSize > 3)
			{
				DisplayString("            QUAD!");
			}
			
			for (j = 0; j < lPolygonSize; j++)
			{
				int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

				Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);

				for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
				{
					FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor( l);
					FBXSDK_sprintf(header, 100, "            Color vertex: "); 

					switch (leVtxc->GetMappingMode())
					{
					case FbxGeometryElement::eByControlPoint:
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
								DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
						{
							switch (leVtxc->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
								DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leVtxc->GetIndexArray().GetAt(vertexId);
									DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
								}
								break;
							default:
								break; // other reference modes not shown here!
							}
						}
						break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}
				for (l = 0; l < pMesh->GetElementUVCount(); ++l)
				{
					FbxGeometryElementUV* leUV = pMesh->GetElementUV( l);
					FBXSDK_sprintf(header, 100, "            Texture UV: "); 

					switch (leUV->GetMappingMode())
					{
					case FbxGeometryElement::eByControlPoint:
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
								Display2DVector(header, leUV->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
						{
							int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
							switch (leUV->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
								{
									Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
								}
								break;
							default:
								break; // other reference modes not shown here!
							}
						}
						break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}
				for( l = 0; l < pMesh->GetElementNormalCount(); ++l)
				{
					FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);
					FBXSDK_sprintf(header, 100, "            Normal: "); 

					if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leNormal->GetIndexArray().GetAt(vertexId);
								Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}

				}
				for( l = 0; l < pMesh->GetElementTangentCount(); ++l)
				{
					FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent( l);
					FBXSDK_sprintf(header, 100, "            Tangent: "); 

					if(leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leTangent->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leTangent->GetIndexArray().GetAt(vertexId);
								Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}

				}
				for( l = 0; l < pMesh->GetElementBinormalCount(); ++l)
				{

					FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal( l);

					FBXSDK_sprintf(header, 100, "            Binormal: "); 
					if(leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leBinormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leBinormal->GetIndexArray().GetAt(vertexId);
								Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
				}
				vertexId++;
			} // for polygonSize
			
		} // for polygonCount


		//check visibility for the edges of the mesh
		for(int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
		{
			FbxGeometryElementVisibility* leVisibility=pMesh->GetElementVisibility(l);
			FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
			DisplayString(header);
			switch(leVisibility->GetMappingMode())
			{
				//should be eByEdge
			case FbxGeometryElement::eByEdge:
				//should be eDirect
				for(int j=0; j!=pMesh->GetMeshEdgeCount();++j)
				{
					DisplayInt("        Edge ", j);
					DisplayBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
				}

				break;
			}
		}
		DisplayString("");
	}

	void DisplayTextureNames( FbxProperty &pProperty, FbxString& pConnectionString )
	{
		int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
		if(lLayeredTextureCount > 0)
		{
			for(int j=0; j<lLayeredTextureCount; ++j)
			{
				FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
				int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
				pConnectionString += " Texture ";

				for(int k =0; k<lNbTextures; ++k)
				{
					//lConnectionString += k;
					pConnectionString += "\"";
					pConnectionString += (char*)lLayeredTexture->GetName();
					pConnectionString += "\"";
					pConnectionString += " ";
				}
				pConnectionString += "of ";
				pConnectionString += pProperty.GetName();
				pConnectionString += " on layer ";
				pConnectionString += j;
			}
			pConnectionString += " |";
		}
		else
		{
			//no layered texture simply get on the property
			int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

			if(lNbTextures > 0)
			{
				pConnectionString += " Texture ";
				pConnectionString += " ";

				for(int j =0; j<lNbTextures; ++j)
				{
					FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
					if(lTexture)
					{
						pConnectionString += "\"";
						pConnectionString += (char*)lTexture->GetName();
						pConnectionString += "\"";
						pConnectionString += " ";
					}
				}
				pConnectionString += "of ";
				pConnectionString += pProperty.GetName();
				pConnectionString += " |";
			}
		}
	}
	void DisplayMaterialTextureConnections( FbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l )
	{
		if(!pMaterial)
			return;

		FbxString lConnectionString = "            Material %d -- ";
		//Show all the textures

		FbxProperty lProperty;
		//Diffuse Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		DisplayTextureNames(lProperty, lConnectionString);

		//DiffuseFactor Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
		DisplayTextureNames(lProperty, lConnectionString);

		//Emissive Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
		DisplayTextureNames(lProperty, lConnectionString);

		//EmissiveFactor Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
		DisplayTextureNames(lProperty, lConnectionString);


		//Ambient Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
		DisplayTextureNames(lProperty, lConnectionString); 

		//AmbientFactor Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
		DisplayTextureNames(lProperty, lConnectionString);          

		//Specular Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
		DisplayTextureNames(lProperty, lConnectionString);  

		//SpecularFactor Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
		DisplayTextureNames(lProperty, lConnectionString);

		//Shininess Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
		DisplayTextureNames(lProperty, lConnectionString);

		//Bump Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
		DisplayTextureNames(lProperty, lConnectionString);

		//Normal Map Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
		DisplayTextureNames(lProperty, lConnectionString);

		//Transparent Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
		DisplayTextureNames(lProperty, lConnectionString);

		//TransparencyFactor Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
		DisplayTextureNames(lProperty, lConnectionString);

		//Reflection Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
		DisplayTextureNames(lProperty, lConnectionString);

		//ReflectionFactor Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
		DisplayTextureNames(lProperty, lConnectionString);

		unsigned int len = lConnectionString.GetLen();
		//Update header with material info
		bool lStringOverflow = (len + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
		if (lStringOverflow)
		{
			// Truncate string!
			lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
			lConnectionString = lConnectionString + "...";
		}
		sprintf_s(header, MAT_HEADER_LENGTH, lConnectionString.Buffer(), pMatId, l);
//		FBXSDK_sprintf(header, 100, lConnectionString.Buffer(), pMatId, l);
		DisplayString(header);
	}
	void DisplayMaterialConnections(FbxMesh* pMesh)
	{
		int i, l, lPolygonCount = pMesh->GetPolygonCount();

		char header[MAT_HEADER_LENGTH];

		DisplayString("    Polygons Material Connections");

		//check whether the material maps with only one mesh
		bool lIsAllSame = true;
		for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{

			FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
			if( lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon) 
			{
				lIsAllSame = false;
				break;
			}
		}

		//For eAllSame mapping type, just out the material and texture mapping info once
		if(lIsAllSame)
		{
			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{

				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
				if( lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame) 
				{
					FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));    
					int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
					if(lMatId >= 0)
					{
						DisplayInt("        All polygons share the same material in mesh ", l);
						DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
					}
				}
			}

			//no material
			if(l == 0)
				DisplayString("        no material applied");
		}

		//For eByPolygon mapping type, just out the material and texture mapping info once
		else
		{
			for (i = 0; i < lPolygonCount; i++)
			{
				DisplayInt("        Polygon ", i);

				for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
				{

					FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
					FbxSurfaceMaterial* lMaterial = NULL;
					int lMatId = -1;
					lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
					lMatId = lMaterialElement->GetIndexArray().GetAt(i);

					if(lMatId >= 0)
					{
						DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
					}
				}
			}
		}
	}
	void DisplayMaterialMapping(FbxMesh* pMesh)
	{
		char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
		char* lReferenceMode[] = { "Direct", "Index", "Index to Direct"};

		int lMtrlCount = 0;
		FbxNode* lNode = NULL;
		if(pMesh){
			lNode = pMesh->GetNode();
			if(lNode)
				lMtrlCount = lNode->GetMaterialCount();    
		}

		for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{
			FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial( l);
			if (leMat)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "    Material Element %d: ", l); 
				DisplayString(header);


				DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
				DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);

				int lMaterialCount = 0;
				FbxString lString;

				if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
					leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					lMaterialCount = lMtrlCount;
				}

				if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
					leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					int i;

					lString = "           Indices: ";

					int lIndexArrayCount = leMat->GetIndexArray().GetCount(); 
					for (i = 0; i < lIndexArrayCount; i++)
					{
						lString += leMat->GetIndexArray().GetAt(i);

						if (i < lIndexArrayCount - 1)
						{
							lString += ", ";
						}
					}

					lString += "\n";

					printf(lString);
				}
			}
		}

		DisplayString("");
	}
	void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode)
	{
		FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pTexture);
		FbxProceduralTexture *lProceduralTexture = FbxCast<FbxProceduralTexture>(pTexture);

		DisplayString("            Name: \"", (char *) pTexture->GetName(), "\"");
		if (lFileTexture)
		{
			DisplayString("            Type: File Texture");
			DisplayString("            File Name: \"", (char *) lFileTexture->GetFileName(), "\"");
		}
		else if (lProceduralTexture)
		{
			DisplayString("            Type: Procedural Texture");
		}
		DisplayDouble("            Scale U: ", pTexture->GetScaleU());
		DisplayDouble("            Scale V: ", pTexture->GetScaleV());
		DisplayDouble("            Translation U: ", pTexture->GetTranslationU());
		DisplayDouble("            Translation V: ", pTexture->GetTranslationV());
		DisplayBool("            Swap UV: ", pTexture->GetSwapUV());
		DisplayDouble("            Rotation U: ", pTexture->GetRotationU());
		DisplayDouble("            Rotation V: ", pTexture->GetRotationV());
		DisplayDouble("            Rotation W: ", pTexture->GetRotationW());

		char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

		DisplayString("            Alpha Source: ", lAlphaSources[pTexture->GetAlphaSource()]);
		DisplayDouble("            Cropping Left: ", pTexture->GetCroppingLeft());
		DisplayDouble("            Cropping Top: ", pTexture->GetCroppingTop());
		DisplayDouble("            Cropping Right: ", pTexture->GetCroppingRight());
		DisplayDouble("            Cropping Bottom: ", pTexture->GetCroppingBottom());

		char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
			"Box", "Face", "UV", "Environment" };

		DisplayString("            Mapping Type: ", lMappingTypes[pTexture->GetMappingType()]);

		if (pTexture->GetMappingType() == FbxTexture::ePlanar)
		{
			char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

			DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]);
		}

		char* lBlendModes[]   = { "Translucent", "Add", "Modulate", "Modulate2" };   
		if(pBlendMode >= 0)
			DisplayString("            Blend Mode: ", lBlendModes[pBlendMode]);
		DisplayDouble("            Alpha: ", pTexture->GetDefaultAlpha());

		if (lFileTexture)
		{
			char* lMaterialUses[] = { "Model Material", "Default Material" };
			DisplayString("            Material Use: ", lMaterialUses[lFileTexture->GetMaterialUse()]);
		}

		char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map", 
			"Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

		DisplayString("            Texture Use: ", pTextureUses[pTexture->GetTextureUse()]);
		DisplayString("");                

	}
	void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex){

		if( pProperty.IsValid() )
		{
			int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();

			for (int j = 0; j < lTextureCount; ++j)
			{
				//Here we have to check if it's layeredtextures, or just textures:
				FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
				if (lLayeredTexture)
				{
					DisplayInt("    Layered Texture: ", j);
					FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
					int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
					for(int k =0; k<lNbTextures; ++k)
					{
						FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
						if(lTexture)
						{

							if(pDisplayHeader){                    
								DisplayInt("    Textures connected to Material ", pMaterialIndex);
								pDisplayHeader = false;
							}

							//NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
							//Why is that?  because one texture can be shared on different layered textures and might
							//have different blend modes.

							FbxLayeredTexture::EBlendMode lBlendMode;
							lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
							DisplayString("    Textures for ", pProperty.GetName());
							DisplayInt("        Texture ", k);  
							DisplayTextureInfo(lTexture, (int) lBlendMode);   
						}

					}
				}
				else
				{
					//no layered texture simply get on the property
					FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
					if(lTexture)
					{
						//display connected Material header only at the first time
						if(pDisplayHeader){                    
							DisplayInt("    Textures connected to Material ", pMaterialIndex);
							pDisplayHeader = false;
						}             

						DisplayString("    Textures for ", pProperty.GetName());
						DisplayInt("        Texture ", j);  
						DisplayTextureInfo(lTexture, -1);
					}
				}
			}
		}//end if pProperty

	}
	void DisplayTexture(FbxGeometry* pGeometry)
	{
		int lMaterialIndex;
		FbxProperty lProperty;
		if(pGeometry->GetNode()==NULL)
		{
			AIEPrintf("We have this many surface materials: %i\n",pGeometry->GetSrcObjectCount<FbxSurfaceMaterial>());
			AIEPrintf("We have this many textures: %i\n",pGeometry->GetSrcObjectCount<FbxTexture>());
			return;
		}
		int lNbMat = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
		for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++){
			FbxSurfaceMaterial *lMaterial = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
			bool lDisplayHeader = true;

			//go through all the possible textures
			if(lMaterial){

				int lTextureIndex;
				FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
				{
					lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
					FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, lMaterialIndex); 
				}

			}//end if(lMaterial)

		}// end for lMaterialIndex     
	}
	void DisplayMarker(FbxNode* pNode)
	{
		FbxMarker* lMarker = (FbxMarker*)pNode->GetNodeAttribute();
		FbxString     lString;

		DisplayString("Marker Name: ", (char *) pNode->GetName());
		DisplayMetaDataConnections(lMarker);

		// Type
		lString = "    Marker Type: ";
		switch (lMarker->GetType())
		{
		case FbxMarker::eStandard:    lString += "Standard";    break;
		case FbxMarker::eOptical:     lString += "Optical";     break;
		case FbxMarker::eEffectorIK: lString += "IK Effector"; break;
		case FbxMarker::eEffectorFK: lString += "FK Effector"; break;
		}
		DisplayString(lString.Buffer());

		// Look
		lString = "    Marker Look: ";
		switch (lMarker->Look.Get())
		{
		case FbxMarker::eCube:         lString += "Cube";        break;
		case FbxMarker::eHardCross:   lString += "Hard Cross";  break;
		case FbxMarker::eLightCross:  lString += "Light Cross"; break;
		case FbxMarker::eSphere:       lString += "Sphere";      break;
		}
		DisplayString(lString.Buffer());

		// Size
		lString = FbxString("    Size: ") + FbxString(lMarker->Size.Get());
		DisplayString(lString.Buffer());

		// Color
		FbxDouble3 c = lMarker->Color.Get();
		FbxColor color(c[0], c[1], c[2]);
		DisplayColor("    Color: ", color);

		// IKPivot
		Display3DVector("    IKPivot: ", lMarker->IKPivot.Get());
	}

	void DisplayProperties(FbxObject* pObject);
	void DisplayGenericInfo(FbxNode* pNode, int pDepth);
	void DisplayGenericInfo(FbxScene* pScene)
	{
		int i;
		FbxNode* lRootNode = pScene->GetRootNode();

		for( i = 0; i < lRootNode->GetChildCount(); i++)
		{
			DisplayGenericInfo(lRootNode->GetChild(i), 0);
		}

		//Other objects directly connected onto the scene
		for( i = 0; i < pScene->GetSrcObjectCount<FbxObject>(); ++i )
		{
			DisplayProperties(pScene->GetSrcObject<FbxObject>(i));
		}
	}
	void DisplayGenericInfo(FbxNode* pNode, int pDepth)
	{
		FbxString lString;
		int i;

		for(i = 0; i < pDepth; i++)
		{
			lString += "     ";
		}

		lString += pNode->GetName();
		lString += "\n";

		DisplayString(lString.Buffer());

		//Display generic info about that Node
		DisplayProperties(pNode);
		DisplayString("");
		for(i = 0; i < pNode->GetChildCount(); i++)
		{
			DisplayGenericInfo(pNode->GetChild(i), pDepth + 1);
		}
	}

	void DisplayProperties(FbxObject* pObject)
	{

		DisplayString("Name: ", (char *)pObject->GetName());

		// Display all the properties
		int i,  lCount = 0;
		FbxProperty lProperty = pObject->GetFirstProperty();
		while (lProperty.IsValid())
		{
			lCount++;
			lProperty = pObject->GetNextProperty(lProperty);
		}

		FbxString lTitleStr = "    Property Count: ";

		if (lCount == 0)
			return; // there are no properties to display

		DisplayInt(lTitleStr.Buffer(), lCount);

		i=0;
		lProperty = pObject->GetFirstProperty();
		while (lProperty.IsValid())
		{
			// exclude user properties

			FbxString lString;
			DisplayInt("        Property ", i);
			lString = lProperty.GetLabel();
			DisplayString("            Display Name: ", lString.Buffer());
			lString = lProperty.GetName();
			DisplayString("            Internal Name: ", lString.Buffer());
			lString = lProperty.GetPropertyDataType().GetName();
			DisplayString("            Type: ",lString.Buffer());
			if (lProperty.HasMinLimit()) DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
			if (lProperty.HasMaxLimit()) DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
			DisplayBool  ("            Is Animatable: ", lProperty.GetFlag(FbxPropertyAttr::eAnimatable));
			DisplayBool  ("            Is Temporary: ", lProperty.GetFlag(FbxPropertyAttr::eTemporary));


			switch (lProperty.GetPropertyDataType().GetType())
			{
			case eFbxBool:
				DisplayBool("            Default Value: ", lProperty.Get<FbxBool>());
				break;

			case eFbxDouble:
				DisplayDouble("            Default Value: ", lProperty.Get<FbxDouble>());
				break;

			case eFbxDouble4:
				{
					FbxColor lDefault;
					char      lBuf[64];

					lDefault = lProperty.Get<FbxColor>();
					FBXSDK_sprintf(lBuf, 64, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
					DisplayString("            Default Value: ", lBuf);
				}
				break;

			case eFbxInt:
				DisplayInt("            Default Value: ", lProperty.Get<FbxInt>());
				break;

			case eFbxDouble3:
				{
					FbxDouble3 lDefault;
					char   lBuf[64];

					lDefault = lProperty.Get<FbxDouble3>();
					FBXSDK_sprintf(lBuf, 64, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
					DisplayString("            Default Value: ", lBuf);
				}
				break;

				//case FbxEnumDT:
				//    DisplayInt("            Default Value: ", lProperty.Get<FbxEnum>());
				//    break;

			case eFbxFloat:
				DisplayDouble("            Default Value: ", lProperty.Get<FbxFloat>());
				break;
			case eFbxString:
				lString = lProperty.Get<FbxString>();
				DisplayString("            Default Value: ", lString.Buffer());
				break;

			default:
				DisplayString("            Default Value: UNIDENTIFIED");
				break;
			}
			i++;
			lProperty = pObject->GetNextProperty(lProperty);
		}
	}
	
	void DisplayUserProperties(FbxObject* pObject)
	{
		int lCount = 0;
		FbxString lTitleStr = "    Property Count: ";

		FbxProperty lProperty = pObject->GetFirstProperty();
		while (lProperty.IsValid())
		{
			if (lProperty.GetFlag(FbxPropertyAttr::eUser))
				lCount++;

			lProperty = pObject->GetNextProperty(lProperty);
		}

		if (lCount == 0)
			return; // there are no user properties to display

		DisplayInt(lTitleStr.Buffer(), lCount);

		lProperty = pObject->GetFirstProperty();
		int i = 0;
		while (lProperty.IsValid())
		{
			if (lProperty.GetFlag(FbxPropertyAttr::eUser))
			{
				DisplayInt("        Property ", i);
				FbxString lString = lProperty.GetLabel();
				DisplayString("            Display Name: ", lString.Buffer());
				lString = lProperty.GetName();
				DisplayString("            Internal Name: ", lString.Buffer());
				DisplayString("            Type: ", lProperty.GetPropertyDataType().GetName());
				if (lProperty.HasMinLimit()) DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
				if (lProperty.HasMaxLimit()) DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
				DisplayBool  ("            Is Animatable: ", lProperty.GetFlag(FbxPropertyAttr::eAnimatable));
				DisplayBool  ("            Is Temporary: ", lProperty.GetFlag(FbxPropertyAttr::eTemporary));

				FbxDataType lPropertyDataType=lProperty.GetPropertyDataType();

				// BOOL
				if (lPropertyDataType.GetType() == eFbxBool)
				{
					DisplayBool("            Default Value: ", lProperty.Get<FbxBool>());
				}
				// REAL
				else if (lPropertyDataType.GetType() == eFbxDouble || lPropertyDataType.GetType() == eFbxFloat)
				{
					DisplayDouble("            Default Value: ", lProperty.Get<FbxDouble>());
				}
				// COLOR
				else if (lPropertyDataType.Is(FbxColor3DT) || lPropertyDataType.Is(FbxColor4DT))
				{
					FbxColor lDefault;
					char      lBuf[64];

					lDefault = lProperty.Get<FbxColor>();
					FBXSDK_sprintf(lBuf, 64, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
					DisplayString("            Default Value: ", lBuf);
				}
				// INTEGER
				else if (lPropertyDataType.GetType() == eFbxInt)
				{
					DisplayInt("            Default Value: ", lProperty.Get<FbxInt>());
				}
				// VECTOR
				else if(lPropertyDataType.GetType() == eFbxDouble3 || lPropertyDataType.GetType() == eFbxDouble4)
				{
					FbxDouble3 lDefault;
					char   lBuf[64];

					lDefault = lProperty.Get<FbxDouble3>();
					FBXSDK_sprintf(lBuf, 64, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
					DisplayString("            Default Value: ", lBuf);
				}
				// LIST
				else if (lPropertyDataType.GetType() == eFbxEnum)
				{
					DisplayInt("            Default Value: ", lProperty.Get<FbxEnum>());
				}
				// UNIDENTIFIED
				else
				{
					DisplayString("            Default Value: UNIDENTIFIED");
				}
				i++;
			}

			lProperty = pObject->GetNextProperty(lProperty);
		}
	}
	void DisplayPivotsAndLimits(FbxNode* pNode)
	{
		FbxVector4 lTmpVector;

		//
		// Pivots
		//
		AIEPrintf("    Pivot Information\n");

		FbxNode::EPivotState lPivotState;
		pNode->GetPivotState(FbxNode::eSourcePivot, lPivotState);
		AIEPrintf("        Pivot State: %s\n", lPivotState == FbxNode::ePivotActive ? "Active" : "Reference");

		lTmpVector = pNode->GetPreRotation(FbxNode::eSourcePivot);
		AIEPrintf("        Pre-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		lTmpVector = pNode->GetPostRotation(FbxNode::eSourcePivot);
		AIEPrintf("        Post-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		lTmpVector = pNode->GetRotationPivot(FbxNode::eSourcePivot);
		AIEPrintf("        Rotation Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		lTmpVector = pNode->GetRotationOffset(FbxNode::eSourcePivot);
		AIEPrintf("        Rotation Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		lTmpVector = pNode->GetScalingPivot(FbxNode::eSourcePivot);
		AIEPrintf("        Scaling Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		lTmpVector = pNode->GetScalingOffset(FbxNode::eSourcePivot);
		AIEPrintf("        Scaling Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		//
		// Limits
		//
		bool        lIsActive, lMinXActive, lMinYActive, lMinZActive;
		bool        lMaxXActive, lMaxYActive, lMaxZActive;
		FbxDouble3  lMinValues, lMaxValues;

		AIEPrintf("    Limits Information\n");

		lIsActive = pNode->TranslationActive;
		lMinXActive = pNode->TranslationMinX;
		lMinYActive = pNode->TranslationMinY;
		lMinZActive = pNode->TranslationMinZ;
		lMaxXActive = pNode->TranslationMaxX;
		lMaxYActive = pNode->TranslationMaxY;
		lMaxZActive = pNode->TranslationMaxZ;
		lMinValues = pNode->TranslationMin;
		lMaxValues = pNode->TranslationMax;

		AIEPrintf("        Translation limits: %s\n", lIsActive ? "Active" : "Inactive");
		AIEPrintf("            X\n");
		AIEPrintf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[0]);
		AIEPrintf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[0]);
		AIEPrintf("            Y\n");
		AIEPrintf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[1]);
		AIEPrintf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[1]);
		AIEPrintf("            Z\n");
		AIEPrintf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[2]);
		AIEPrintf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[2]);

		lIsActive = pNode->RotationActive;
		lMinXActive = pNode->RotationMinX;
		lMinYActive = pNode->RotationMinY;
		lMinZActive = pNode->RotationMinZ;
		lMaxXActive = pNode->RotationMaxX;
		lMaxYActive = pNode->RotationMaxY;
		lMaxZActive = pNode->RotationMaxZ;
		lMinValues = pNode->RotationMin;
		lMaxValues = pNode->RotationMax;

		AIEPrintf("        Rotation limits: %s\n", lIsActive ? "Active" : "Inactive");
		AIEPrintf("            X\n");
		AIEPrintf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[0]);
		AIEPrintf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[0]);
		AIEPrintf("            Y\n");
		AIEPrintf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[1]);
		AIEPrintf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[1]);
		AIEPrintf("            Z\n");
		AIEPrintf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[2]);
		AIEPrintf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[2]);

		lIsActive = pNode->ScalingActive;
		lMinXActive = pNode->ScalingMinX;
		lMinYActive = pNode->ScalingMinY;
		lMinZActive = pNode->ScalingMinZ;
		lMaxXActive = pNode->ScalingMaxX;
		lMaxYActive = pNode->ScalingMaxY;
		lMaxZActive = pNode->ScalingMaxZ;
		lMinValues = pNode->ScalingMin;
		lMaxValues = pNode->ScalingMax;

		AIEPrintf("        Scaling limits: %s\n", lIsActive ? "Active" : "Inactive");
		AIEPrintf("            X\n");
		AIEPrintf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[0]);
		AIEPrintf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[0]);
		AIEPrintf("            Y\n");
		AIEPrintf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[1]);
		AIEPrintf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[1]);
		AIEPrintf("            Z\n");
		AIEPrintf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
		AIEPrintf("                Min Limit Value: %f\n", lMinValues[2]);
		AIEPrintf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
		AIEPrintf("                Max Limit Value: %f\n", lMaxValues[2]);
	}
		/*
	void DisplayGlobalTimeSettings(FbxGlobalSettings* pGlobalSettings)
	{
	char lTimeString[256];

	DisplayString("Time Mode : ", FbxGetTimeModeName(pGlobalSettings->GetTimeMode()));

	FbxTimeSpan lTs;
	FbxTime     lStart, lEnd;
	pGlobalSettings->GetTimelineDefaultTimeSpan(lTs);
	lStart = lTs.GetStart();
	lEnd   = lTs.GetStop();
	DisplayString("Timeline default timespan: ");
	DisplayString("     Start: ", lStart.GetTimeString(lTimeString, FbxUShort(256)));
	DisplayString("     Stop : ", lEnd.GetTimeString(lTimeString, FbxUShort(256)));

	DisplayString("");
	}
	*/
}