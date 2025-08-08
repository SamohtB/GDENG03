import unreal
import re

def create_actor(object_data, folder_path):
    """Creates a single actor in the Unreal world based on parsed data."""
    actor_type = unreal.StaticMeshActor
    mesh_path = ""
    mesh_type_str = object_data.get("mesh_type")

    if mesh_type_str == "PRIMITIVE_CUBE":
        mesh_path = "/Engine/BasicShapes/Cube.Cube"
    elif mesh_type_str == "PRIMITIVE_PLANE":
        mesh_path = "/Engine/BasicShapes/Plane.Plane"
    elif mesh_type_str == "PRIMITIVE_SPHERE":
        mesh_path = "/Engine/BasicShapes/Sphere.Sphere"
    elif mesh_type_str == "PRIMITIVE_CYLINDER":
        mesh_path = "/Engine/BasicShapes/Cylinder.Cylinder"
    elif mesh_type_str == "PRIMITIVE_CAPSULE":
        mesh_path = "/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"
    else:
        unreal.log_warning(f"Unknown mesh type '{mesh_type_str}' for object {object_data.get('name')}. Skipping.")
        return

    world = unreal.EditorLevelLibrary.get_editor_world()
    
    location = unreal.Vector(*object_data.get("position", [0, 0, 0]))
    quat_vals = object_data.get("rotation", [0, 0, 0, 1])
    rotation = unreal.Quat(*quat_vals).rotator()
    scale = unreal.Vector(*object_data.get("scale", [1, 1, 1]))

    if mesh_type_str == "PRIMITIVE_PLANE":
        scale *= 10.0
    elif mesh_type_str == "PRIMITIVE_CYLINDER":
        scale.z *= 2.0

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(actor_type, location, rotation)
    actor.set_actor_label(object_data.get("name", "DefaultName"))

    static_mesh_component = actor.get_component_by_class(unreal.StaticMeshComponent)
    static_mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
    
    if static_mesh_component and static_mesh:
        static_mesh_component.set_static_mesh(static_mesh)
        
        # --- MODIFIED RIGIDBODY LOGIC ---
        # Read the rigidbody type and set mobility and physics accordingly
        rigidbody_type = object_data.get("rigidbody_type", "Static")
        
        if rigidbody_type == "Dynamic":
            static_mesh_component.set_mobility(unreal.ComponentMobility.MOVABLE)
            static_mesh_component.set_simulate_physics(True)
        else: # Default to Static for "Static" or any other value
            static_mesh_component.set_mobility(unreal.ComponentMobility.STATIC)
            static_mesh_component.set_simulate_physics(False)

    actor.set_actor_scale3d(scale)
    actor.set_folder_path(folder_path)
    unreal.log(f"Created '{object_data.get('name')}' as {rigidbody_type}.")

def parse_level_file(file_path):
    """Parses a .level file and spawns actors for each [GameObject] block."""
    try:
        with open(file_path, 'r') as file:
            content = file.read()
    except FileNotFoundError:
        unreal.log_error(f"File not found: {file_path}")
        return

    game_object_blocks = re.split(r'(\[GameObject\])', content)
    folder_path = "ImportedFromLevel"

    for i in range(1, len(game_object_blocks), 2):
        block = game_object_blocks[i+1]
        current_object = {}
        
        for line in block.strip().split('\n'):
            if ':' not in line:
                continue
            
            key, value = [p.strip() for p in line.split(':', 1)]

            if key == "Name":
                current_object["name"] = value
            elif key == "Position":
                pos = [float(p) for p in value.split()]
                current_object["position"] = [pos[2] * 100.0, pos[0] * 100.0, pos[1] * 100.0]
            elif key == "Rotation":
                rot = [float(r) for r in value.split()]
                current_object["rotation"] = [rot[2], rot[0], rot[1], rot[3]]
            elif key == "Scale":
                scl = [float(s) for s in value.split()]
                current_object["scale"] = [scl[2], scl[0], scl[1]]
            elif key == "MeshType":
                current_object["mesh_type"] = value
            # --- MODIFIED PARSING LOGIC ---
            elif key == "RigidbodyType":
                current_object["rigidbody_type"] = value

        if "name" in current_object and "mesh_type" in current_object:
            create_actor(current_object, folder_path)

# --- Main Execution ---
level_file_path = r"E:\Documents\School Stuff\DLSU Stuff\GDENG03\GDENG03\Scenes\UnrealTest.level"
parse_level_file(level_file_path)