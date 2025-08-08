import unreal

# ----- CONFIG -----
# change to your scene file path
SCENE_FILE_PATH = r"D:\GitRepository\GDENG03\Scenes\UnrealScene_4.level" 

# Primitive mesh assets
PRIMITIVE_ASSETS = {
    "PRIMITIVE_PLANE": "/Engine/BasicShapes/Plane.Plane",
    "PRIMITIVE_CUBE": "/Engine/BasicShapes/Cube.Cube",
    "PRIMITIVE_SPHERE": "/Engine/BasicShapes/Sphere.Sphere",
    "PRIMITIVE_CYLINDER": "/Engine/BasicShapes/Cylinder.Cylinder",
    "PRIMITIVE_CAPSULE": "/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"
}


# ----- FUNCTIONS -----
def load_scene_text(file_path):
    with open(file_path, "r") as f:
        return f.read()

def parse_scene_data(scene_text):
    objects = []
    blocks = scene_text.split("[GameObject]")
    for block in blocks:
        block = block.strip()
        if not block:
            continue
        obj = {}
        for line in block.splitlines():
            if ":" not in line:
                continue
            key, value = [part.strip() for part in line.split(":", 1)]

            if key == "Name":
                obj["name"] = value

            elif key == "Position":
                # Unity (x,y,z meters) → Unreal (Z,X,Y cm)
                pos = [float(p) for p in value.split()]
                obj["position"] = [pos[2] * 100.0, pos[0] * 100.0, pos[1] * 100.0]

            elif key == "Rotation":
                # Unity quaternion (x,y,z,w) → Unreal quaternion (z,x,y,w)
                rot = [float(r) for r in value.split()]
                obj["rotation"] = [rot[2], rot[0], rot[1], rot[3]]

            elif key == "Scale":
                # Unity scale (x,y,z) → Unreal scale (z,x,y)
                scl = [float(s) for s in value.split()]
                obj["scale"] = [scl[2], scl[0], scl[1]]

            elif key == "MeshType":
                obj["mesh_type"] = value

            elif key == "RigidbodyType":
                obj["rigidbody_type"] = value

        if "name" in obj and "mesh_type" in obj:
            objects.append(obj)

    return objects

def spawn_game_object(obj_data):
    mesh_path = PRIMITIVE_ASSETS.get(obj_data["mesh_type"])
    if not mesh_path:
        unreal.log_warning(f"Unknown mesh type: {obj_data['mesh_type']}")
        return

    mesh_asset = unreal.EditorAssetLibrary.load_asset(mesh_path)
    if not mesh_asset:
        unreal.log_warning(f"Mesh not found: {mesh_path}")
        return

    # Build transforms
    location = unreal.Vector(*obj_data["position"])
    quat_vals = obj_data["rotation"]
    rotation = unreal.Quat(*quat_vals).rotator()
    scale = unreal.Vector(*obj_data["scale"])

    # Adjust sizes for some primitives
    if obj_data["mesh_type"] == "PRIMITIVE_PLANE":
        scale *= 10.0
    elif obj_data["mesh_type"] == "PRIMITIVE_CAPSULE":
        scale *= 2.0

    # Spawn
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    actor.set_actor_label(obj_data["name"])

    static_mesh_comp = actor.static_mesh_component
    static_mesh_comp.set_static_mesh(mesh_asset)

    # Rigidbody type handling
    rb_type = obj_data.get("rigidbody_type", "Static")
    if rb_type == "Dynamic":
        static_mesh_comp.set_mobility(unreal.ComponentMobility.MOVABLE)
        static_mesh_comp.set_simulate_physics(True)
    elif rb_type == "Kinematic":
        static_mesh_comp.set_mobility(unreal.ComponentMobility.MOVABLE)
        static_mesh_comp.set_simulate_physics(False)
    else:  # Static or None
        static_mesh_comp.set_mobility(unreal.ComponentMobility.STATIC)
        static_mesh_comp.set_simulate_physics(False)

    actor.set_actor_scale3d(scale)
    unreal.log(f"Spawned '{obj_data['name']}' with RigidbodyType: {rb_type}")

# ----- MAIN -----
def main():
    scene_text = load_scene_text(SCENE_FILE_PATH)
    objects = parse_scene_data(scene_text)

    for obj in objects:
        spawn_game_object(obj)

main()