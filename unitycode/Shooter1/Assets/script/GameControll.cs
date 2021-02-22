using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameControll : MonoBehaviour
{
    public GameObject[] prefabs;
    public float speed;
    private float lastTime;

    [System.Serializable]
    public struct Boundary
    {
        public float xMin;
        public float xMax;
        public float zMin;
        public float zMax;
    }


    public Boundary boundary;

    private void Awake()
    {
        instance = this;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update() 
    {
        if (Time.time - lastTime > speed) {
            var obj = GameObject.Instantiate(prefabs[Random.RandomRange(0, prefabs.Length)]);
            obj.transform.position = new Vector3(Random.RandomRange(boundary.xMin,boundary.xMax),0.68F,10);
            lastTime = Time.time;
        }
    }


    private static GameControll instance;

    public static GameControll Instance
    {
        get {
            return instance;
        }
      
    }
}
