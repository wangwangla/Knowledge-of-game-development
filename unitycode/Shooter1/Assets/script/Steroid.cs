using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Steroid : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        GetComponent<Rigidbody>().velocity = Vector3.back * 3;
        var dir = Random.insideUnitCircle;
        GetComponent<Rigidbody>().angularVelocity = new Vector3(dir.x, 0, dir.y) * 6;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
