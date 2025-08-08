import unreal

def resolve_mesh_type(actor):
    component = actor.get_component_by_class(unreal.StaticMeshComponent)
    if not component or not component.static_mesh:
        return "PRIMITIVE_NONE"

    mesh_id = component.static_mesh.get_name()
    if "Cube" in mesh_id:
        return "PRIMITIVE_CUBE"
    elif "Plane" in mesh_id:
        return "PRIMITIVE_PLANE"
    elif "Sphere" in mesh_id:
        return "PRIMITIVE_SPHERE"
    elif "Cylinder" in mesh_id:
        return "PRIMITIVE_CYLINDER"
    return "PRIMITIVE_UNKNOWN"

def write_scene_to_file(path, actor_list):
    with open(path, "w") as output:
        for actor in actor_list:
            output.write("[GameObject]\n")
            output.write(f"Name: {actor.get_actor_label()}\n")
            output.write("ParentName: None\n")

            pos = actor.get_actor_location()
            output.write(f"Position: {pos.y / 100.0} {pos.z / 100.0} {pos.x / 100.0}\n")

            rot = actor.get_actor_rotation().quaternion()
            output.write(f"Rotation: {rot.y} {rot.z} {rot.x} {rot.w}\n")

            scale = actor.get_actor_scale3d()
            mesh_type = resolve_mesh_type(actor)

            if mesh_type == "PRIMITIVE_PLANE":
                scale /= 10.0
            elif mesh_type == "PRIMITIVE_CYLINDER":
                scale.z /= 2.0

            output.write(f"Scale: {scale.y} {scale.z} {scale.x}\n")
            output.write(f"MeshType: {mesh_type}\n")

            comp = actor.get_component_by_class(unreal.StaticMeshComponent)
            physics = comp.is_simulating_physics() if comp else False
            output.write(f"HasRigidbody: {str(physics).lower()}\n\n")

    unreal.log(f"[Exporter] Scene written to: {path}")

group_label = "ImportedFromLevel"
all_actors = unreal.EditorLevelLibrary.get_all_level_actors()
target_actors = [actor for actor in all_actors if actor.get_folder_path() == group_label]

if target_actors:
    target_file = r"D:\GitRepository\GDENG03\Scenes\Output.level"
    write_scene_to_file(target_file, target_actors)
    unreal.log(f"[Exporter] Wrote {len(target_actors)} actors from folder '{group_label}'")
else:
    unreal.log_warning(f"[Exporter] No actors found in folder '{group_label}'")
