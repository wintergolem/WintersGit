using UnityEngine;
using System.Collections;

public class FPCScript : MonoBehaviour {
	
	Vector3 cLocation, hLocation; //current location and holding location(used for telek power)
	Vector3 hMove; //movenment away from camera for holding
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		
		SetLocationVaribles();
	}
	
	void SetLocationVaribles()
	{
		cLocation = transform.position;

	}
}
