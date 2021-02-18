using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Buttlet : MonoBehaviour
{
    Rigidbody body;
    private void Awake()
    {
        body = GetComponent<Rigidbody>();
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        body.velocity = Vector3.forward * 10;
    }
}
