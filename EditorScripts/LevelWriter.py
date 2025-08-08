import unreal

# ----- CONFIG -----
SAVE_FILE_PATH = r"D:\GitRepository\GDENG03\Scenes\UnrealScene_5.level"

# Mesh type detection
def get_mesh_type_string(actor):
    static_mesh_component = actor.get_component_by_class(unreal.StaticMeshComponent)
    if not static_mesh_component:
        return "PRIMITIVE_NONE"

    static_mesh = static_mesh_component.static_mesh
    if not static_mesh:
        return "PRIMITIVE_NONE"

    mesh_name = static_mesh.get_name()
    if "Cube" in mesh_name:
        return "PRIMITIVE_CUBE"
    elif "Plane" in mesh_name:
        return "PRIMITIVE_PLANE"
    elif "Sphere" in mesh_name:
        return "PRIMITIVE_SPHERE"
    elif "Cylinder" in mesh_name:
        return "PRIMITIVE_CYLINDER"
    elif "Capsule" in mesh_name:
        return "PRIMITIVE_CAPSULE"
    else:
        return "PRIMITIVE_UNKNOWN"

# Rigidbody type detection (matching loader format)
def get_rigidbody_type(actor):
    static_mesh_component = actor.get_component_by_class(unreal.StaticMeshComponent)
    if not static_mesh_component:
        return "Static"

    if static_mesh_component.is_simulating_physics():
        return "Dynamic"
    elif static_mesh_component.mobility == unreal.ComponentMobility.MOVABLE:
        return "Kinematic"
    else:
        return "Static"

# Save scene to .level
def save_scene_to_level_file(file_path, actors_to_save):
    with open(file_path, 'w') as file:
        for actor in actors_to_save:
            file.write("[GameObject]\n")
            file.write(f"Name: {actor.get_actor_label()}\n")
            file.write("ParentName: None\n")

            # Position (Unreal cm → Unity m + axis swap)
            loc = actor.get_actor_location()
            file.write(f"Position: {loc.y / 100.0} {loc.z / 100.0} {loc.x / 100.0}\n")

            # Rotation (Unreal → Unity quaternion order)
            rot_quat = actor.get_actor_rotation().quaternion()
            file.write(f"Rotation: {rot_quat.y} {rot_quat.z} {rot_quat.x} {rot_quat.w}\n")

            # Scale (axis swap + primitive-specific fixes)
            scale = actor.get_actor_scale3d()
            mesh_type = get_mesh_type_string(actor)
            if mesh_type == "PRIMITIVE_PLANE":
                scale /= 10.0
            elif mesh_type == "PRIMITIVE_CYLINDER":
                scale.z /= 2.0
            elif mesh_type == "PRIMITIVE_CAPSULE":
                scale /= 2.0
            file.write(f"Scale: {scale.y} {scale.z} {scale.x}\n")

            # Mesh type
            file.write(f"MeshType: {mesh_type}\n")

            # Rigidbody type
            rigidbody_type = get_rigidbody_type(actor)
            file.write(f"RigidbodyType: {rigidbody_type}\n\n")

    unreal.log(f"Scene successfully saved to {file_path}")

# --- Main Execution ---
all_level_actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()

# Filter only actors with a StaticMeshComponent (prevents empty entries)
actors_to_export = [
    a for a in all_level_actors 
    if a.get_component_by_class(unreal.StaticMeshComponent) is not None
]

if actors_to_export:
    save_scene_to_level_file(SAVE_FILE_PATH, actors_to_export)
    unreal.log(f"Saved {len(actors_to_export)} mesh actors to {SAVE_FILE_PATH}")
else:
    unreal.log_warning("No mesh actors found in the level. Nothing to save.")