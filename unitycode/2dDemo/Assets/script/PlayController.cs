using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayController : MonoBehaviour
{
    public float speed = 5;

    private Rigidbody2D rigidbody;
    // Start is called before the first frame update
    void Start()
    {
        rigidbody = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        //自己运动
       // transform.Translate(transform.right * speed * Time.deltaTime);

        //按键控制运动
        float moveX = Input.GetAxisRaw("Horizontal");
        float moveY = Input.GetAxisRaw("Vertical");
        // 两个都没有错，v2它会去掉
        Vector3  vv = transform.position;  
        Vector2 position = transform.position;
        position.x += moveX * speed*Time.deltaTime;
        position.y += moveY * speed* Time.deltaTime;
        //   transform.position = position;

        //   rigidbody.position = position;
        rigidbody.MovePosition(position);
    }
}
