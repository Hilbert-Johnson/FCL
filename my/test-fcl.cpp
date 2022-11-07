#include "../build/include/fcl/fcl.h"
#include "../test/test_fcl_utility.h"

int num_max_contacts = std::numeric_limits<int>::max();

void loadOBJFile(const char *filename, std::vector<fcl::Vector3d> &posVec, std::vector<fcl::Vector3<int>> &triVec) {
      FILE *file = fopen(filename, "rb");
      if (!file) {
        std::cerr << "file not exist:" << filename << std::endl;
        return;
      }

      bool has_normal = false;
      bool has_texture = false;
      char line_buffer[2000];
      while (fgets(line_buffer, 2000, file)) {
        char *first_token = strtok(line_buffer, "\r\n\t ");
        if (!first_token || first_token[0] == '#' || first_token[0] == 0)
          continue;

        switch (first_token[0]) {
          case 'v': {
            if (first_token[1] == 'n') {
              strtok(nullptr, "\t ");
              strtok(nullptr, "\t ");
              strtok(nullptr, "\t ");
              has_normal = true;
            } else if (first_token[1] == 't') {
              strtok(nullptr, "\t ");
              strtok(nullptr, "\t ");
              has_texture = true;
            } else {
              double x = atof(strtok(nullptr, "\t "));
              double y = atof(strtok(nullptr, "\t "));
              double z = atof(strtok(nullptr, "\t "));

              posVec.emplace_back(x, y, z);
            }
          }
            break;
          case 'f': {
//        Triangle tri;
            int vIdx[3];
            char *data[30];
            int n = 0;
            while ((data[n] = strtok(nullptr, "\t \r\n")) != nullptr) {
              if (strlen(data[n]))
                n++;
            }

            for (int t = 0; t < (n - 2); ++t) {
              if ((!has_texture) && (!has_normal)) {
                vIdx[0] = atoi(data[0]) - 1;
                vIdx[1] = atoi(data[1]) - 1;
                vIdx[2] = atoi(data[2]) - 1;
              } else {
                const char *v1;
                for (int i = 0; i < 3; i++) {
                  // vertex ID
                  if (i == 0)
                    v1 = data[0];
                  else
                    v1 = data[t + i];
                  vIdx[i] = atoi(v1) - 1;
                }
              }
              triVec.emplace_back(vIdx[0], vIdx[1], vIdx[2]);
            }
          }
        }
      }
      fclose(file);
      std::printf("Finished loading obj file %s: vertex %zu triangles: %zu\n", filename, posVec.size(),
                  triVec.size());
    }

int main(){
    std::vector<fcl::Vector3d> vertices;
    std::vector<fcl::Triangle> triangles;
    fcl::test::loadOBJFile("/home/ubuntu/fcl/my/tshirt1000-tri.obj", vertices, triangles);
    double high = 0.0;
    double low = 100.0;
    for(int i =0; i<vertices.size(); i++){
      if(vertices[i][1]>high){
        high = vertices[i][1];
      }
      if(vertices[i][1]<low){
        low = vertices[i][1];
      }
    }

    std::cout<<high<<std::endl;
    std::cout<<low<<std::endl;
    while(1){
      
    }

    typedef fcl::BVHModel<fcl::OBBRSSd> Model;
    std::shared_ptr<Model> model = std::make_shared<Model>();

    model->beginModel();
    model->addSubModel(vertices, triangles);
    model->endModel();

    fcl::Transform3d pose1 = fcl::Transform3d::Identity();
    fcl::CollisionObjectd* obj1 = new fcl::CollisionObjectd(model, pose1);
    
    fcl::Vector3d T(1.001, 0, 0);
    fcl::Transform3d pose2 = fcl::Transform3d::Identity();
    pose2.translation() = T;
    std::shared_ptr<fcl::Sphered> sphere = std::make_shared<fcl::Sphered>(0.001);
    fcl::CollisionObjectd* obj2 = new fcl::CollisionObjectd(sphere, pose2);

    fcl::CollisionRequestd request(num_max_contacts, true);
    fcl::CollisionResultd result;
    int ret = fcl::collide(obj1, obj2, request, result);
    std::cout<<"contact number: "<<ret<<std::endl;
    std::vector<fcl::Contactd> contacts;
    result.getContacts(contacts);
    for(int i=0; i<contacts.size(); i++){
        std::cout<<i<<"th contact position: ["<<contacts[i].pos[0]<<", "<<contacts[i].pos[1]<<", "<<contacts[i].pos[2]<<"]"<<std::endl;
        std::cout<<i<<"th penetration depth: "<<contacts[i].penetration_depth<<std::endl;
        std::cout<<i<<"th contact normal: ["<<contacts[i].normal[0]<<", "<<contacts[i].normal[1]<<", "<<contacts[i].normal[2]<<"]"<<std::endl<<std::endl;
    }
    return 0;
}

