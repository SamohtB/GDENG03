import unreal

def get_mesh_type_string(actor):
    """Returns the PRIMITIVE_* string based on the actor's static mesh."""
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
    # ❗ ADDED: Return PRIMITIVE_CAPSULE if the mesh is a capsule
    elif "Capsule" in mesh_name:
        return "PRIMITIVE_CAPSULE"
    else:
        return "PRIMITIVE_UNKNOWN"

def save_scene_to_level_file(file_path, actors_to_save):
    """Writes a list of actors to a .level file with correct formatting and transformations."""
    with open(file_path, 'w') as file:
        for actor in actors_to_save:
            file.write("[GameObject]\n")
            
            file.write(f"Name: {actor.get_actor_label()}\n")
            file.write("ParentName: None\n")
            
            location = actor.get_actor_location()
            file.write(f"Position: {location.y / 100.0} {location.z / 100.0} {location.x / 100.0}\n")
            
            rotation_rotator = actor.get_actor_rotation()
            rotation_quat = rotation_rotator.quaternion()
            file.write(f"Rotation: {rotation_quat.y} {rotation_quat.z} {rotation_quat.x} {rotation_quat.w}\n")
            
            scale = actor.get_actor_scale3d()
            mesh_type = get_mesh_type_string(actor)

            # Note: No scale correction is applied for capsules, assuming a 1:1 mapping.
            if mesh_type == "PRIMITIVE_PLANE":
                scale /= 10.0
            elif mesh_type == "PRIMITIVE_CYLINDER":
                scale.z /= 2.0
            elif mesh_type == "PRIMITIVE_CAPSULE":
                scale /= 2.0


            file.write(f"Scale: {scale.y} {scale.z} {scale.x}\n")
            file.write(f"MeshType: {mesh_type}\n")

            static_mesh_component = actor.get_component_by_class(unreal.StaticMeshComponent)
            
            # FIXED: This is the correct method to check the physics state.
            has_rigidbody = static_mesh_component.is_simulating_physics() if static_mesh_component else False
            file.write(f"HasRigidbody: {str(has_rigidbody).lower()}\n\n")
            
    unreal.log(f"Scene successfully saved to {file_path}")

# --- Main Execution ---
folder_name = "ImportedFromLevel"
all_level_actors = unreal.EditorLevelLibrary.get_all_level_actors()

actors_to_save = [actor for actor in all_level_actors if actor.get_folder_path() == folder_name]

if actors_to_save:
    output_file_path = "E:/Documents/School Stuff/DLSU Stuff/GDENG03/GDENG03/Scenes/Output.level"
    save_scene_to_level_file(output_file_path, actors_to_save)
    unreal.log(f"Found and saved {len(actors_to_save)} actors from folder '{folder_name}'.")
else:
    unreal.log_warning(f"No actors found in the folder '{folder_name}'. Nothing to save.")