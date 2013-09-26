#include "FBXLoader.h"
#include "fbxsdk.h"

namespace AIE
{
	void AIEPrintf(const char* s, ...);
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

	void DisplayHierarchy(FbxScene* pScene)
	{
		int i;
		FbxNode* lRootNode = pScene->GetRootNode();

		for( i = 0; i < lRootNode->GetChildCount(); i++)
		{
			DisplayHierarchy(lRootNode->GetChild(i), 0);
		}
	}

	void DisplayHierarchy(FbxNode* pNode, int pDepth)
	{
		FbxString lString;
		int i;

		for(i = 0; i < pDepth; i++)
		{
			lString += "     ";
		}

		lString += pNode->GetName();
		lString += "\n";

		AIEPrintf(lString.Buffer());

		for(i = 0; i < pNode->GetChildCount(); i++)
		{
			DisplayHierarchy(pNode->GetChild(i), pDepth + 1);
		}
	}

	void DisplayAnimation(FbxScene* pScene)
	{
		int i;
		for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
		{
			FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

			FbxString lOutputString = "Animation Stack Name: ";
			lOutputString += lAnimStack->GetName();
			lOutputString += "\n\n";
			AIEPrintf(lOutputString);

	//		DisplayAnimation(lAnimStack, pScene->GetRootNode(), true);
			DisplayAnimation(lAnimStack, pScene->GetRootNode());
		}
	}

	void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
	{
		int l;
		int nbAnimLayers = pAnimStack->GetMemberCount(FbxCriteria::ObjectType(FbxAnimLayer::ClassId));
		FbxString lOutputString;

		lOutputString = "Animation stack contains ";
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer(s)\n";
		AIEPrintf(lOutputString);

		for (l = 0; l < nbAnimLayers; l++)
		{
			FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

			lOutputString = "AnimLayer ";
			lOutputString += l;
			lOutputString += "\n";
			AIEPrintf(lOutputString);

			DisplayAnimation(lAnimLayer, pNode, isSwitcher);
		}
	}

	void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
	{
		int lModelCount;
		FbxString lOutputString;

		lOutputString = "     Node Name: ";
		lOutputString += pNode->GetName();
		lOutputString += "\n\n";
		AIEPrintf(lOutputString);

		DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher);
		AIEPrintf ("\n");

		for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
		{
			DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
		}
	}

	void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher)
	{
		FbxAnimCurve* lAnimCurve = NULL;

		// Display general curves.
		if (!isSwitcher)
		{
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				AIEPrintf("        TX\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				AIEPrintf("        TY\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				AIEPrintf("        TZ\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				AIEPrintf("        RX\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				AIEPrintf("        RY\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				AIEPrintf("        RZ\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				AIEPrintf("        SX\n");
				DisplayCurve(lAnimCurve);
			}    
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				AIEPrintf("        SY\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				AIEPrintf("        SZ\n");
				DisplayCurve(lAnimCurve);
			}
		}
		/*
		// Display curves specific to a light or marker.
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

		if (lNodeAttribute)
		{
			lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_RED);
			if (lAnimCurve)
			{
				AIEPrintf("        Red\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_GREEN);
			if (lAnimCurve)
			{
				AIEPrintf("        Green\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_BLUE);
			if (lAnimCurve)
			{
				AIEPrintf("        Blue\n");
				DisplayCurve(lAnimCurve);
			}

			// Display curves specific to a light.
			FbxLight* light = pNode->GetLight();
			if (light)
			{
				lAnimCurve = light->Intensity.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					AIEPrintf("        Intensity\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = light->OuterAngle.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					AIEPrintf("        Outer Angle\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = light->Fog.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					AIEPrintf("        Fog\n");
					DisplayCurve(lAnimCurve);
				}
			}

			// Display curves specific to a camera.
			FbxCamera* camera = pNode->GetCamera();
			if (camera)
			{
				lAnimCurve = camera->FieldOfView.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					AIEPrintf("        Field of View\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->FieldOfViewX.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					AIEPrintf("        Field of View X\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->FieldOfViewY.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					AIEPrintf("        Field of View Y\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->OpticalCenterX.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					AIEPrintf("        Optical Center X\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->OpticalCenterY.GetCurve(pAnimLayer);
				if(lAnimCurve)
				{
					AIEPrintf("        Optical Center Y\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->Roll.GetCurve(pAnimLayer);
				if(lAnimCurve)
				{
					AIEPrintf("        Roll\n");
					DisplayCurve(lAnimCurve);
				}
			}

			// Display curves specific to a geometry.
			if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
				lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
				lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
			{
				FbxGeometry* lGeometry = (FbxGeometry*) lNodeAttribute;

				int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
				for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
				{
					FbxBlendShape* lBlendShape = (FbxBlendShape*)lGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

					int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
					for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
					{
						FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
						const char* lChannelName = lChannel->GetName();

						lAnimCurve = lGeometry->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer, true);
						if(lAnimCurve)
						{
							AIEPrintf("        Shape %s\n", lChannelName);
							DisplayCurve(lAnimCurve);
						}
					}
				}
			}
		}
		*/
		/*
		// Display curves specific to properties
		FbxProperty lProperty = pNode->GetFirstProperty();
		while (lProperty.IsValid())
		{
			if (lProperty.GetFlag(FbxPropertyAttr::eUser))
			{
				FbxString lFbxFCurveNodeName  = lProperty.GetName();
				FbxAnimCurveNode* lCurveNode = lProperty.GetCurveNode(pAnimLayer);

				if (!lCurveNode){
					lProperty = pNode->GetNextProperty(lProperty);
					continue;
				}

				FbxDataType lDataType = lProperty.GetPropertyDataType();
				if (lDataType.GetType() == eFbxBool || lDataType.GetType() == eFbxDouble || lDataType.GetType() == eFbxFloat || lDataType.GetType() == eFbxInt)
				{
					FbxString lMessage;

					lMessage =  "        Property ";
					lMessage += lProperty.GetName();
					if (lProperty.GetLabel().GetLen() > 0)
					{
						lMessage += " (Label: ";
						lMessage += lProperty.GetLabel();
						lMessage += ")";
					};

					DisplayString(lMessage.Buffer());

					for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(0U, c);
						if (lAnimCurve)
							DisplayCurve(lAnimCurve);
					}
				}
				else if(lDataType.GetType() == eFbxDouble3 || lDataType.GetType() == eFbxDouble4 || lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT))
				{
					char* lComponentName1 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_RED : (char*)"X";
					char* lComponentName2 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_GREEN : (char*)"Y";
					char* lComponentName3 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_BLUE  : (char*)"Z";
					FbxString      lMessage;

					lMessage =  "        Property ";
					lMessage += lProperty.GetName();
					if (lProperty.GetLabel().GetLen() > 0)
					{
						lMessage += " (Label: ";
						lMessage += lProperty.GetLabel();
						lMessage += ")";
					}
					DisplayString(lMessage.Buffer());

					for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(0U, c);
						if (lAnimCurve)
						{
							DisplayString("        Component ", lComponentName1);
							DisplayCurve(lAnimCurve);
						}
					}

					for( int c = 0; c < lCurveNode->GetCurveCount(1U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(1U, c);
						if (lAnimCurve)
						{
							DisplayString("        Component ", lComponentName2);
							DisplayCurve(lAnimCurve);
						}
					}

					for( int c = 0; c < lCurveNode->GetCurveCount(2U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(2U, c);
						if (lAnimCurve)
						{
							DisplayString("        Component ", lComponentName3);
							DisplayCurve(lAnimCurve);
						}
					}
				}
				else if (lDataType.GetType() == eFbxEnum)
				{
					FbxString lMessage;

					lMessage =  "        Property ";
					lMessage += lProperty.GetName();
					if (lProperty.GetLabel().GetLen() > 0)
					{
						lMessage += " (Label: ";
						lMessage += lProperty.GetLabel();
						lMessage += ")";
					};
					DisplayString(lMessage.Buffer());

					for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(0U, c);
						if (lAnimCurve)
							DisplayListCurve(lAnimCurve, &lProperty);
					}                
				}
			}

			lProperty = pNode->GetNextProperty(lProperty);
		} // while
		*/

	}

	static int InterpolationFlagToIndex(int flags)
	{
		if( (flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant ) return 1;
		if( (flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear ) return 2;
		if( (flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic ) return 3;
		return 0;
	}

	static int ConstantmodeFlagToIndex(int flags)
	{
		if( (flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard ) return 1;
		if( (flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext ) return 2;
		return 0;
	}

	static int TangentmodeFlagToIndex(int flags)
	{
		if( (flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto ) return 1;
		if( (flags & FbxAnimCurveDef::eTangentAutoBreak)== FbxAnimCurveDef::eTangentAutoBreak ) return 2;
		if( (flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB ) return 3;
		if( (flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser ) return 4;
		if( (flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak ) return 5;
		if( (flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak ) return 6;
		return 0;
	}

	static int TangentweightFlagToIndex(int flags)
	{
		if( (flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone ) return 1;
		if( (flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight ) return 2;
		if( (flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft ) return 3;
		return 0;
	}

	static int TangentVelocityFlagToIndex(int flags)
	{
		if( (flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone ) return 1;
		if( (flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight ) return 2;
		if( (flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft ) return 3;
		return 0;
	}

	void DisplayCurveKeys(FbxAnimCurve* pCurve)
	{
		static char* interpolation[] = { "?", "constant", "linear", "cubic"};
		static char* constantMode[] =  { "?", "Standard", "Next" };
		static char* cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
		static char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

		FbxTime   lKeyTime;
		float   lKeyValue;
		char    lTimeString[256];
		FbxString lOutputString;
		int     lCount;

		int lKeyCount = pCurve->KeyGetCount();

		for(lCount = 0; lCount < lKeyCount; lCount++)
		{
			lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
			lKeyTime  = pCurve->KeyGetTime(lCount);

			lOutputString = "            Key Time: ";
			lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
			lOutputString += ".... Key Value: ";
			lOutputString += lKeyValue;
			lOutputString += " [ ";
			lOutputString += interpolation[ InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount)) ];
			if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
			{
				lOutputString += " | ";
				lOutputString += constantMode[ ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount)) ];
			}
			else if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
			{
				lOutputString += " | ";
				lOutputString += cubicMode[ TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount)) ];
				lOutputString += " | ";
				lOutputString += tangentWVMode[ TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode()) ];
				lOutputString += " | ";
				lOutputString += tangentWVMode[ TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode()) ];
			}
			lOutputString += " ]";
			lOutputString += "\n";
			AIEPrintf (lOutputString);
		}
	}

	void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
	{
		FbxTime   lKeyTime;
		int     lKeyValue;
		char    lTimeString[256];
		FbxString lListValue;
		FbxString lOutputString;
		int     lCount;

		int lKeyCount = pCurve->KeyGetCount();

		for(lCount = 0; lCount < lKeyCount; lCount++)
		{
			lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
			lKeyTime  = pCurve->KeyGetTime(lCount);

			lOutputString = "            Key Time: ";
			lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
			lOutputString += ".... Key Value: ";
			lOutputString += lKeyValue;
			lOutputString += " (";
			lOutputString += pProperty->GetEnumValue(lKeyValue);
			lOutputString += ")";

			lOutputString += "\n";
			AIEPrintf (lOutputString);
		}
	}

	void DisplayPose(FbxScene* pScene)
	{
		int      i,j,k,lPoseCount;
		FbxString  lName;

		lPoseCount = pScene->GetPoseCount();

		for (i = 0; i < lPoseCount; i++)
		{
			FbxPose* lPose = pScene->GetPose(i);

			lName = lPose->GetName();
			DisplayString("Pose Name: ", lName.Buffer());

			DisplayBool("    Is a bind pose: ", lPose->IsBindPose());

			DisplayInt("    Number of items in the pose: ", lPose->GetCount());

			DisplayString("","");

			for (j=0; j<lPose->GetCount(); j++)
			{
				lName = lPose->GetNodeName(j).GetCurrentName();
				DisplayString("    Item name: ", lName.Buffer());

				if (!lPose->IsBindPose())
				{
					// Rest pose can have local matrix
					DisplayBool("    Is local space matrix: ", lPose->IsLocalMatrix(j));
				}

				DisplayString("    Matrix value: ","");

				FbxString lMatrixValue;

				for (k=0; k<4; k++)
				{
					FbxMatrix  lMatrix = lPose->GetMatrix(j);
					FbxVector4 lRow = lMatrix.GetRow(k);
					char        lRowValue[1024];

					FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
					lMatrixValue += FbxString("        ") + FbxString(lRowValue);
				}

				DisplayString("", lMatrixValue.Buffer());
			}
		}

		lPoseCount = pScene->GetCharacterPoseCount();

		for (i = 0; i < lPoseCount; i++)
		{
			FbxCharacterPose* lPose = pScene->GetCharacterPose(i);
			FbxCharacter*     lCharacter = lPose->GetCharacter();

			if (!lCharacter) break;

			DisplayString("Character Pose Name: ", lCharacter->GetName());

			FbxCharacterLink lCharacterLink;
			FbxCharacter::ENodeId  lNodeId = FbxCharacter::eHips;

			while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
			{
				FbxAMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);

				DisplayString("    Matrix value: ","");

				FbxString lMatrixValue;

				for (k=0; k<4; k++)
				{
					FbxVector4 lRow = lGlobalPosition.GetRow(k);
					char        lRowValue[1024];

					FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
					lMatrixValue += FbxString("        ") + FbxString(lRowValue);
				}

				DisplayString("", lMatrixValue.Buffer());

				lNodeId = FbxCharacter::ENodeId(int(lNodeId) + 1);
			}
		}
	}

	void DisplayLink(FbxGeometry* pGeometry)
	{
		//Display cluster now

		//int i, lLinkCount;
		//FbxCluster* lLink;

		int i, j;
		int lSkinCount=0;
		int lClusterCount=0;
		FbxCluster* lCluster;

		lSkinCount=pGeometry->GetDeformerCount(FbxDeformer::eSkin);

		//lLinkCount = pGeometry->GetLinkCount();
		for(i=0; i!=lSkinCount; ++i)
		{
			lClusterCount = ((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
			for (j = 0; j != lClusterCount; ++j)
			{
				DisplayInt("    Cluster ", i);

				lCluster=((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
				//lLink = pGeometry->GetLink(i);    

				char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

				DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

				if(lCluster->GetLink() != NULL)
				{
					DisplayString("        Name: ", (char *) lCluster->GetLink()->GetName());
				}

				FbxString lString1 = "        Link Indices: ";
				FbxString lString2 = "        Weight Values: ";

				int k, lIndexCount = lCluster->GetControlPointIndicesCount();
				int* lIndices = lCluster->GetControlPointIndices();
				double* lWeights = lCluster->GetControlPointWeights();

				for(k = 0; k < lIndexCount; k++)
				{
					lString1 += lIndices[k];
					lString2 += (float) lWeights[k];

					if (k < lIndexCount - 1)
					{
						lString1 += ", ";
						lString2 += ", ";
					}
				}

				lString1 += "\n";
				lString2 += "\n";

				AIEPrintf(lString1);
				AIEPrintf(lString2);

				DisplayString("");

				FbxAMatrix lMatrix;

				lMatrix = lCluster->GetTransformMatrix(lMatrix);
				Display3DVector("        Transform Translation: ", lMatrix.GetT());
				Display3DVector("        Transform Rotation: ", lMatrix.GetR());
				Display3DVector("        Transform Scaling: ", lMatrix.GetS());

				lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
				Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
				Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
				Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

				if (lCluster->GetAssociateModel() != NULL)
				{
					lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
					DisplayString("        Associate Model: ", (char *) lCluster->GetAssociateModel()->GetName());
					Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
					Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
					Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
				}

				DisplayString("");
			}
		}
	}

	void DisplayShape(FbxGeometry* pGeometry)
	{
		int lBlendShapeCount, lBlendShapeChannelCount, lTargetShapeCount;
		FbxBlendShape* lBlendShape;
		FbxBlendShapeChannel* lBlendShapeChannel;
		FbxShape* lShape;

		lBlendShapeCount = pGeometry->GetDeformerCount(FbxDeformer::eBlendShape);

		for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeCount; ++lBlendShapeIndex)
		{
			lBlendShape = (FbxBlendShape*) pGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);   
			DisplayString("    BlendShape ", (char *) lBlendShape->GetName());

			lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
			for(int lBlendShapeChannelIndex = 0;  lBlendShapeChannelIndex < lBlendShapeChannelCount; ++lBlendShapeChannelIndex)
			{
				lBlendShapeChannel = lBlendShape->GetBlendShapeChannel(lBlendShapeChannelIndex);
				DisplayString("    BlendShapeChannel ", (char *) lBlendShapeChannel->GetName());
				DisplayDouble("    Default Deform Value: ", lBlendShapeChannel->DeformPercent.Get());

				lTargetShapeCount = lBlendShapeChannel->GetTargetShapeCount();
				for (int lTargetShapeIndex = 0; lTargetShapeIndex < lTargetShapeCount; ++lTargetShapeIndex)
				{
					lShape = lBlendShapeChannel->GetTargetShape(lTargetShapeIndex);
					DisplayString("    TargetShape ", (char *) lShape->GetName());

					int j, lControlPointsCount = lShape->GetControlPointsCount();
					FbxVector4* lControlPoints = lShape->GetControlPoints();
					FbxLayerElementArrayTemplate<FbxVector4>* lNormals = NULL;    
					bool lStatus = lShape->GetNormals(&lNormals); 

					for(j = 0; j < lControlPointsCount; j++)
					{
						DisplayInt("        Control Point ", j);
						Display3DVector("            Coordinates: ", lControlPoints[j]);

						if (lStatus && lNormals && lNormals->GetCount() == lControlPointsCount)
						{
							Display3DVector("            Normal Vector: ", lNormals->GetAt(j));
						}
					}

					DisplayString("");
				}
			}
		}
	}

	void DisplaySkeleton(FbxNode* pNode)
	{ 
		FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();

		DisplayString("Skeleton Name: ", (char *) pNode->GetName());
		DisplayMetaDataConnections(lSkeleton);

		char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

		DisplayString("    Type: ", lSkeletonTypes[lSkeleton->GetSkeletonType()]);

		if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
		{
			DisplayDouble("    Limb Length: ", lSkeleton->LimbLength.Get());
		}
		else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
		{
			DisplayDouble("    Limb Node Size: ", lSkeleton->Size.Get());
		}
		else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
		{
			DisplayDouble("    Limb Root Size: ", lSkeleton->Size.Get());
		}

		DisplayColor("    Color: ", lSkeleton->GetLimbNodeColor());
	}
}