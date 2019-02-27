using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PortalTextureSetup : MonoBehaviour {

    public Camera cameraA;
    public Material cameraMatA;

    public Camera cameraB;
    public Material cameraMatB;

	// Use this for initialization
	void Start () {
        Setup(cameraA, cameraMatA);
        Setup(cameraB, cameraMatB);
    }

    private void Setup(Camera _camera, Material _material) {
        if (_camera.targetTexture != null) {
            _camera.targetTexture.Release();
        }
        _camera.targetTexture = new RenderTexture(Screen.width, Screen.height, 24);
        _material.mainTexture = _camera.targetTexture;
    }

}
