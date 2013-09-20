using UnityEngine;
using System.Collections;

public class MinionNodeTransversal : MonoBehaviour {
	
	enum whichPath{top, mid,bot};
	public GameObject MinionPrefab;
	public Transform botSpawnPoint;
	GameObject[] NodeListBot;
	
	// Use this for initialization
	void Start () {
		NodeListBot = GameObject.FindGameObjectWithTag("NodeListBot");
		botSpawnPoint = NodeListBot[0].transform;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	void SpawnMinion()
	{
		Instantiate(MinionPrefab, botSpawnPoint.transform.position, botSpawnPoint.transfrom.position);
	}
}
