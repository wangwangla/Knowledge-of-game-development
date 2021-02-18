﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public struct Boundary {
    public float xMin;
    public float xMax;
    public float zMin;
    public float zMax;
} 

public class Player : MonoBehaviour
{
    public float movedSpeed;
    private Rigidbody playBody;
    public Boundary boundary;

    private float lastTime;

    public Transform firePos;
    public GameObject bulletObject;
    // Start is called before the first frame update
    void Start()
    {
        //獲取剛體組件
        this.playBody = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        if (playBody != null)
        {
            //通過输入来移动   方式一：使用transform    方式二：使用刚体设置力 
            var hor = Input.GetAxis("Horizontal");
            var ver = Input.GetAxis("Vertical");
            playBody.velocity = new Vector3(hor, 0, ver) * 10;
        }


        var posX = Mathf.Clamp(transform.position.x, boundary.xMin, boundary.xMax);
        var posZ = Mathf.Clamp(transform.position.z, boundary.zMin, boundary.zMax);
        transform.position = new Vector3(posX, transform.position.y, posZ);

        if (Time.time - lastTime > 0.3F)
        {
            lastTime = Time.time;
            var bullet = GameObject.Instantiate(bulletObject);
            bullet.transform.position = firePos.position;
     
        }
    }
}