using UnityEngine;
using System.Collections;

public class MoveableObj : MonoBehaviour {
	bool selected = false;
	GameObject owner;
	Vector3 cLocation, tLocation, speedFloat, speedFly;
	public float smoothTime = 0.3F;
    private Vector3 velocity = Vector3.zero;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		GrabTargetLocation();
		transform.position = 
			Vector3.SmoothDamp(transform.position, tLocation, ref velocity, smoothTime);
		
	}
	
	//set selected to true
	void MakeSelected(GameObject caller)//one who called the function
	{
		if(selected == false)
		{
				owner = caller;
		}
	}
	
	void GrabTargetLocation()
	{
		tLocation = owner.transform.position;	
	}
}
