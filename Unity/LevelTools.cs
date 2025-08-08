using UnityEngine;
using UnityEditor; // Required for editor scripts
using System.IO;
using System.Text;
using System.Globalization;
using System.Collections.Generic;

// This attribute runs the static constructor when the editor is launched, caching our meshes.
[InitializeOnLoad]
public class LevelTools
{
    // --- Caches for built-in Unity primitive meshes ---
    private static Mesh planeMesh;
    private static Mesh cubeMesh;
    private static Mesh cylinderMesh;
    private static Mesh sphereMesh;
    private static Mesh capsuleMesh;
    private static Mesh quadMesh;

    // Static constructor is called once by [InitializeOnLoad] on editor startup.
    static LevelTools()
    {
        InitializeMeshCache();
    }

    #region Exporter

    [MenuItem("Tools/Save Scene to .level File")]
    public static void SaveSceneToFile()
    {
        // Find all active GameObjects in the scene.
        GameObject[] allSceneObjects = Object.FindObjectsOfType<GameObject>();
        var objectsToExport = new List<GameObject>();

        // Filter for objects with a recognized primitive mesh.
        foreach (var sceneObject in allSceneObjects)
        {
            // We only want to export root-level objects or objects whose parents are not primitives.
            if (sceneObject.transform.parent != null && GetPrimitiveTypeName(sceneObject.transform.parent.gameObject) != "CUSTOM") continue;

            string meshType = GetPrimitiveTypeName(sceneObject);
            if (meshType != "CUSTOM" && meshType != "None")
            {
                objectsToExport.Add(sceneObject);
            }
        }

        if (objectsToExport.Count == 0)
        {
            EditorUtility.DisplayDialog("No Primitives Found", "Could not find any GameObjects with primitive meshes (Cube, Sphere, etc.) to export.", "OK");
            return;
        }

        string path = EditorUtility.SaveFilePanel("Save Scene as .level", "", "MyLevel.level", "level");

        if (string.IsNullOrEmpty(path))
        {
            return; // User cancelled the save dialog
        }

        var exportSet = new HashSet<GameObject>(objectsToExport);
        StringBuilder sb = new StringBuilder();

        foreach (var go in objectsToExport)
        {
            if (sb.Length > 0)
            {
                sb.AppendLine();
            }

            sb.AppendLine("[GameObject]");
            sb.AppendLine($"Name: {go.name}");

            // Determine parent's name, but only if the parent is also being exported.
            string parentName = "None";
            if (go.transform.parent != null && exportSet.Contains(go.transform.parent.gameObject))
            {
                parentName = go.transform.parent.name;
            }
            sb.AppendLine($"ParentName: {parentName}");

            // Use invariant culture to ensure consistent number formatting (e.g., using '.' as decimal separator)
            sb.AppendLine($"Position: {go.transform.position.x.ToString(CultureInfo.InvariantCulture)} {go.transform.position.y.ToString(CultureInfo.InvariantCulture)} {go.transform.position.z.ToString(CultureInfo.InvariantCulture)}");
            sb.AppendLine($"Rotation: {go.transform.rotation.x.ToString(CultureInfo.InvariantCulture)} {go.transform.rotation.y.ToString(CultureInfo.InvariantCulture)} {go.transform.rotation.z.ToString(CultureInfo.InvariantCulture)} {go.transform.rotation.w.ToString(CultureInfo.InvariantCulture)}");
            sb.AppendLine($"Scale: {go.transform.localScale.x.ToString(CultureInfo.InvariantCulture)} {go.transform.localScale.y.ToString(CultureInfo.InvariantCulture)} {go.transform.localScale.z.ToString(CultureInfo.InvariantCulture)}");
            sb.AppendLine($"MeshType: {GetPrimitiveTypeName(go)}");

            // --- MODIFIED: Determine and write RigidbodyType ---
            Rigidbody rb = go.GetComponent<Rigidbody>();
            string rigidbodyType;
            if (rb == null)
            {
                rigidbodyType = "Static";
            }
            else
            {
                rigidbodyType = rb.isKinematic ? "Kinematic" : "Dynamic";
            }
            sb.AppendLine($"RigidbodyType: {rigidbodyType}");
        }

        File.WriteAllText(path, sb.ToString());
        AssetDatabase.Refresh();
        EditorUtility.DisplayDialog("Export Successful", $"{objectsToExport.Count} primitive objects exported to:\n{path}", "OK");
    }

    /// <summary>
    /// Caches Unity's built-in meshes on editor load for reliable comparison.
    /// </summary>
    private static void InitializeMeshCache()
    {
        GameObject tempPlane = GameObject.CreatePrimitive(PrimitiveType.Plane);
        planeMesh = tempPlane.GetComponent<MeshFilter>().sharedMesh;
        GameObject tempCube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        cubeMesh = tempCube.GetComponent<MeshFilter>().sharedMesh;
        GameObject tempCylinder = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
        cylinderMesh = tempCylinder.GetComponent<MeshFilter>().sharedMesh;
        GameObject tempSphere = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        sphereMesh = tempSphere.GetComponent<MeshFilter>().sharedMesh;
        GameObject tempCapsule = GameObject.CreatePrimitive(PrimitiveType.Capsule);
        capsuleMesh = tempCapsule.GetComponent<MeshFilter>().sharedMesh;
        GameObject tempQuad = GameObject.CreatePrimitive(PrimitiveType.Quad);
        quadMesh = tempQuad.GetComponent<MeshFilter>().sharedMesh;

        Object.DestroyImmediate(tempPlane);
        Object.DestroyImmediate(tempCube);
        Object.DestroyImmediate(tempCylinder);
        Object.DestroyImmediate(tempSphere);
        Object.DestroyImmediate(tempCapsule);
        Object.DestroyImmediate(tempQuad);
    }

    /// <summary>
    /// Identifies mesh type by comparing against the cached built-in primitive meshes.
    /// </summary>
    private static string GetPrimitiveTypeName(GameObject go)
    {
        MeshFilter mf = go.GetComponent<MeshFilter>();
        if (mf == null || mf.sharedMesh == null) return "None";

        Mesh mesh = mf.sharedMesh;

        if (mesh == planeMesh) return "PRIMITIVE_PLANE";
        if (mesh == cubeMesh) return "PRIMITIVE_CUBE";
        if (mesh == cylinderMesh) return "PRIMITIVE_CYLINDER";
        if (mesh == sphereMesh) return "PRIMITIVE_SPHERE";
        if (mesh == capsuleMesh) return "PRIMITIVE_CAPSULE";
        if (mesh == quadMesh) return "PRIMITIVE_QUAD";

        return "CUSTOM";
    }

    #endregion

    #region Importer

    [MenuItem("Tools/Load Level From File")]
    public static void LoadLevelFromFile()
    {
        string path = EditorUtility.OpenFilePanel("Select .level file", "", "level");
        if (string.IsNullOrEmpty(path)) return;

        string[] lines = File.ReadAllLines(path);
        string levelName = Path.GetFileNameWithoutExtension(path);

        ParseAndCreateGameObjects(lines, levelName);
    }

    /// <summary>
    /// Parses lines from the level file and creates the GameObjects.
    /// </summary>
    private static void ParseAndCreateGameObjects(string[] lines, string rootName)
    {
        GameObject levelRoot = new GameObject(rootName);
        Undo.RegisterCreatedObjectUndo(levelRoot, "Load Level");

        GameObject currentGameObject = null;
        Dictionary<string, GameObject> createdObjects = new Dictionary<string, GameObject>();

        foreach (string line in lines)
        {
            if (string.IsNullOrWhiteSpace(line)) continue;

            if (line.Trim() == "[GameObject]")
            {
                currentGameObject = new GameObject();
                continue;
            }

            if (currentGameObject != null)
            {
                string[] parts = line.Split(new[] { ':' }, 2);
                if (parts.Length < 2) continue;

                string key = parts[0].Trim();
                string value = parts[1].Trim();

                switch (key)
                {
                    case "Name":
                        currentGameObject.name = value;
                        createdObjects[value] = currentGameObject;
                        break;
                    case "ParentName":
                        if (value != "None" && createdObjects.ContainsKey(value))
                        {
                            currentGameObject.transform.SetParent(createdObjects[value].transform);
                        }
                        else
                        {
                            currentGameObject.transform.SetParent(levelRoot.transform);
                        }
                        break;
                    case "Position":
                        currentGameObject.transform.position = ParseVector3(value);
                        break;
                    case "Rotation":
                        currentGameObject.transform.rotation = ParseQuaternion(value);
                        break;
                    case "Scale":
                        currentGameObject.transform.localScale = ParseVector3(value);
                        break;
                    case "MeshType":
                        CreatePrimitive(currentGameObject, value);
                        break;
                    // --- MODIFIED: Read RigidbodyType and configure component ---
                    case "RigidbodyType":
                        switch (value)
                        {
                            case "Dynamic":
                                // isKinematic is false by default
                                currentGameObject.AddComponent<Rigidbody>();
                                break;
                            case "Kinematic":
                                Rigidbody newRb = currentGameObject.AddComponent<Rigidbody>();
                                newRb.isKinematic = true;
                                break;
                            case "Static":
                                // Do nothing. An object without a Rigidbody is static.
                                break;
                        }
                        break;
                }
            }
        }
    }

    /// <summary>
    /// Creates a primitive mesh for the given GameObject.
    /// </summary>
    private static void CreatePrimitive(GameObject obj, string meshType)
    {
        PrimitiveType primitiveType;
        switch (meshType)
        {
            case "PRIMITIVE_PLANE": primitiveType = PrimitiveType.Plane; break;
            case "PRIMITIVE_CUBE": primitiveType = PrimitiveType.Cube; break;
            case "PRIMITIVE_CYLINDER": primitiveType = PrimitiveType.Cylinder; break;
            case "PRIMITIVE_SPHERE": primitiveType = PrimitiveType.Sphere; break;
            case "PRIMITIVE_CAPSULE": primitiveType = PrimitiveType.Capsule; break;
            default:
                Debug.LogWarning($"Unknown MeshType: {meshType}. Creating an empty GameObject.");
                return;
        }

        GameObject primitive = GameObject.CreatePrimitive(primitiveType);

        obj.AddComponent<MeshFilter>().sharedMesh = primitive.GetComponent<MeshFilter>().sharedMesh;
        obj.AddComponent<MeshRenderer>().sharedMaterial = primitive.GetComponent<MeshRenderer>().sharedMaterial;

        Object.DestroyImmediate(primitive);
    }

    /// <summary>
    /// Parses a string into a Vector3, using invariant culture for consistency.
    /// </summary>
    private static Vector3 ParseVector3(string s)
    {
        string[] parts = s.Split(' ');
        float x = float.Parse(parts[0], CultureInfo.InvariantCulture);
        float y = float.Parse(parts[1], CultureInfo.InvariantCulture);
        float z = float.Parse(parts[2], CultureInfo.InvariantCulture);
        return new Vector3(x, y, z);
    }

    /// <summary>
    /// Parses a string into a Quaternion, using invariant culture for consistency.
    /// </summary>
    private static Quaternion ParseQuaternion(string s)
    {
        string[] parts = s.Split(' ');
        float x = float.Parse(parts[0], CultureInfo.InvariantCulture);
        float y = float.Parse(parts[1], CultureInfo.InvariantCulture);
        float z = float.Parse(parts[2], CultureInfo.InvariantCulture);
        float w = float.Parse(parts[3], CultureInfo.InvariantCulture);
        return new Quaternion(x, y, z, w);
    }

    #endregion
}