#include<fstream>
#include<string>
#include<vector>
#include<unordered_map>

struct VertexS {
    unsigned CurDist = 0xffffffff;
    std::vector<unsigned> Connections;
    bool Stop = false;
};
std::unordered_map<unsigned, VertexS> Vertexes;

int main(int argc, char** argv) {
    
    unsigned GraphVertexesAmount, GraphEdgesAmount, TargetVertex;

    {//read file
        #define CheckStream(stream) if(!stream) return -1;
        std::ifstream InputFile("input.txt", std::ios::in | std::ios::binary);
        if (!InputFile.is_open()) return -1;
        std::string buf;
        CheckStream(std::getline(InputFile, buf)); GraphVertexesAmount = std::stoi(buf);
        CheckStream(std::getline(InputFile, buf)); GraphEdgesAmount = std::stoi(buf);
        for (int i = 0;i < GraphEdgesAmount;i++) {
            CheckStream(std::getline(InputFile, buf));
            size_t spaceInd = buf.find_first_of(' ');
            unsigned ind1 = std::stoi(buf.substr(0, spaceInd)), ind2 = std::stoi(buf.substr(spaceInd + 1));
            Vertexes[ind1].Connections.emplace_back(ind2);
            Vertexes[ind2].Connections.emplace_back(ind1);
        }
        CheckStream(std::getline(InputFile, buf)); TargetVertex = std::stoi(buf);
    }

    {//i will use dexter algorithm
        Vertexes[TargetVertex].CurDist = 0; Vertexes[TargetVertex].Stop = true;
        for (unsigned vert : Vertexes[TargetVertex].Connections) { Vertexes[vert].CurDist = 1; Vertexes[vert].Stop = true; }
        while (true) {
            unsigned min = 0xffffffff;
            for (auto& vert : Vertexes) if (!vert.second.Stop) {
                for (unsigned con : vert.second.Connections)
                    if (Vertexes[con].Stop && vert.second.CurDist > Vertexes[con].CurDist + 1) vert.second.CurDist = Vertexes[con].CurDist + 1;
                if (min > vert.second.CurDist) min = vert.second.CurDist;
            }
            bool allStopped = true;
            for (auto& vert : Vertexes) if (!vert.second.Stop) {
                if (vert.second.CurDist == min) vert.second.Stop = true;
                else allStopped = false;
            }
            if (allStopped) break;
        }
    }

    for (unsigned i = 0;i < GraphVertexesAmount;i++) {
        std::printf("%i\n", Vertexes[i].CurDist);
    }


    return 0;
}