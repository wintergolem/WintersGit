using UnityEngine;
using System.Collections;

public class FPVRaycast : MonoBehaviour {
	RaycastHit hit;
	GUIText HUDErayhit;
	// Use this for initialization
	void Start () {
		HUDErayhit = GameObject.FindGameObjectWithTag("HUDErayhit").guiText;
	}
	
	// Update is called once per frame
	void Update () {
		Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
		if(Physics.Raycast(ray,out hit, 2000000))
		{
			HUDErayhit.text = "Hit";
			if(hit.collider.tag== "Moveable"){
				Debug.Log("hit box");
				HUDErayhit.text = hit.collider.tag;
			}
		}
		else{
			HUDErayhit.text = "Failed to hit";	
		}
	}
}
