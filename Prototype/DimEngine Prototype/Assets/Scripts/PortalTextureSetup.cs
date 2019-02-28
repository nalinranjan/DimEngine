using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PortalTextureSetup : MonoBehaviour {

    public CameraData[] cameraData;

	// Use this for initialization
	void Start () {
        foreach(CameraData cd in cameraData) {
            Setup(cd.camera, cd.material);
        }
    }

    private void Setup(Camera _camera, Material _material) {
        if (_camera.targetTexture != null) {
            _camera.targetTexture.Release();
        }
        _camera.targetTexture = new RenderTexture(Screen.width, Screen.height, 24);
        _material.mainTexture = _camera.targetTexture;
    }

    [Serializable]
    public class CameraData {
        public Camera camera;
        public Material material;
    }

}
