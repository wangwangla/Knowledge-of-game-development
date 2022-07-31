using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BgRound : MonoBehaviour
{
    MeshRenderer renderer;
    public float  speed;
    Vector2 vector2 = new Vector2();
    // Start is called before the first frame update
    void Start()
    {
        renderer = GetComponent<MeshRenderer>();
    }

    // Update is called once per frame
    void Update()
    {
        vector2.Set(0, speed * Time.deltaTime);
        renderer.material.mainTextureOffset += vector2;
    }


    private void OnTriggerExit(Collider other)
    {
        GameObject.Destroy(other.gameObject);
    }
}
