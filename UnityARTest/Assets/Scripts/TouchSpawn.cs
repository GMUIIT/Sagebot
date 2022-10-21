using UnityEngine;

public class TouchSpawn : MonoBehaviour
{
    /**
     * Prefab to be spawned
     */
    public GameObject prefab;

    /**
     * Layers to be used when raycasting
     */
    public LayerMask layers;

    // Update is called once per frame
    private void Update()
    {
        if (Input.touchCount <= 0) return;
        if (Time.frameCount % 4 != 0) return;
        if (Input.touchCount == 3)
        {
            foreach (var obj in GameObject.FindGameObjectsWithTag("Pill"))
            {
                Destroy(obj);
            }

            return;
        }

        var t = Input.GetTouch(0);
        // Raycast from the center of the screen, with a fallback of 3 units in case of no hit
        var ray = Camera.current.ScreenPointToRay(t.position);
        if (Physics.Raycast(ray, out var hit, 30, layers))
        {
            // Spawn the object at the hit position, plus a bit on the y so it falls
            Instantiate(prefab, hit.point + Vector3.up * 0.5f, Quaternion.identity);
        }
        else
        {
            // Spawn the object a small distance away
            Instantiate(prefab, ray.GetPoint(2), Quaternion.identity);
        }
    }
}