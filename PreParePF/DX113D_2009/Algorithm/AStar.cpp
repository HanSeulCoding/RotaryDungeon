#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
    heap = new Heap();
}

AStar::~AStar()
{
    for (Node* node : nodes)
        delete node;

    delete heap;
}

void AStar::Update()
{
    if (KEY_DOWN(VK_RBUTTON))
    {
        Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);

        for (Node* node : nodes)
        {
            if (node->collider->RayCollision(ray))
            {
                node->state = Node::OBSTACLE;
                break;
            }
        }
    }
}

void AStar::Render()
{
    for (Node* node : nodes)
        node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
    Float2 size = terrain->GetSize();

    interval.x = size.x / width;
    interval.y = size.y / height;

    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = terrain->GetHeight(pos);

            int index = z * width + x;
            nodes.emplace_back(new Node(pos, index));
        }
    }

    SetEdge();
}

void AStar::Reset()
{
    for (Node* node : nodes)
    {
        if (node->state != Node::OBSTACLE)
            node->state = Node::NONE;
    }

    heap->Clear();
}

int AStar::FindCloseNode(Vector3 pos)
{
    float minDist = FLT_MAX;
    int index = -1;

    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->state == Node::OBSTACLE)
            continue;

        float dist = Distance(pos, nodes[i]->pos);
        if (minDist > dist)
        {
            minDist = dist;
            index = i;
        }
    }

    return index;
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
    int cutNodeNum = 0;
    Ray ray;
    ray.pos = start;

    for (UINT i = 0; i < path.size(); i++)
    {
        ray.dir = path[i] - ray.pos;
        float distance = ray.dir.Length();

        ray.dir.Normalize();

        if (!CollisionObstacle(ray, distance))
        {
            cutNodeNum = path.size() - i - 1;
            break;
        }
    }

    for (int i = 0; i < cutNodeNum; i++)
        path.pop_back();

    cutNodeNum = 0;
    ray.pos = end;

    for (UINT i = 0; i < path.size(); i++)
    {
        ray.dir = path[path.size() - i - 1] - ray.pos;
        float distance = ray.dir.Length();

        ray.dir.Normalize();

        if (!CollisionObstacle(ray, distance))
        {
            cutNodeNum = path.size() - i - 1;
            break;
        }
    }

    for (int i = 0 ; i < cutNodeNum; i++)
        path.erase(path.begin());
}

bool AStar::CollisionObstacle(Ray ray, float destDistance)
{
    for (Collider* obstacle : obstacles)
    {
        Contact contact;

        if (obstacle->RayCollision(ray, &contact))
        {
            if (contact.distance < destDistance)
                return true;
        }
    }

    return false;
}

vector<Vector3> AStar::GetOrder(int start, int end)
{
    Reset();

    vector<Vector3> order;

    if (nodes[end]->state == Node::OBSTACLE)
        return order;

    float G = 0.0f;
    float H = GetDistance(start, end);

    nodes[start]->g = G;
    nodes[start]->h = H;
    nodes[start]->f = G + H;
    nodes[start]->via = start;
    nodes[start]->state = Node::OPEN;

    heap->Insert(nodes[start]);

    while (nodes[end]->state != Node::CLOSED)
    {
        int curIndex = GetMinNode();
        Extend(curIndex, end);
        nodes[curIndex]->state = Node::CLOSED;
    }

    int curIndex = end;
    while (curIndex != start)
    {
        nodes[curIndex]->state = Node::USING;
        order.emplace_back(nodes[curIndex]->pos);
        curIndex = nodes[curIndex]->via;
    }
    
    heap->Clear();

    return order;
}

void AStar::SetObstacles(vector<class Item*> obstacles)
{
    for (Item* obstacle : obstacles)
    {
        Collider* collider = obstacle->GetCollider();
        this->obstacles.emplace_back(collider);

        for (Node* node : nodes)
        {            
            if (collider->Collision(node->collider))
            {
                node->state = Node::OBSTACLE;
            }
        }
    }
}

float AStar::GetDistance(int start, int end)
{
    Vector3 startPos = nodes[start]->pos;
    Vector3 endPos = nodes[end]->pos;

    Vector3 result = endPos - startPos;

    float x = abs(result.x);
    float y = abs(result.z);

    float minSize = min(x, y);
    float maxSize = max(x, y);

    return sqrt(minSize * minSize * 2) + (maxSize - minSize);
}

int AStar::GetMinNode()
{
    return heap->DeleteRoot()->index;
}

void AStar::Extend(int center, int end)
{
    vector<Node::Edge*> edges = nodes[center]->edges;

    for (Node::Edge* edge : edges)
    {
        int index = edge->index;

        if (nodes[index]->state == Node::CLOSED)
            continue;
        if (nodes[index]->state == Node::OBSTACLE)
            continue;

        float G = nodes[center]->g + edge->cost;
        float H = GetDistance(index, end);
        float F = G + H;

        if (nodes[index]->state == Node::OPEN)
        {
            if (F < nodes[index]->f)
            {
                nodes[index]->g = G;
                nodes[index]->f = F;
                nodes[index]->via = center;
            }
        }
        else if (nodes[index]->state == Node::NONE)
        {
            nodes[index]->g = G;
            nodes[index]->h = H;
            nodes[index]->f = F;
            nodes[index]->via = center;
            nodes[index]->state = Node::OPEN;

            heap->Insert(nodes[index]);
        }
    }
}

void AStar::SetEdge()
{
    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (i % width != width - 1)
        {
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + width]);
            nodes[i + width]->AddEdge(nodes[i]);
        }
        if (i % width != width - 1 && i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + width + 1]);
            nodes[i + width + 1]->AddEdge(nodes[i]);
        }
        if (i % width != 0 && i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + width - 1]);
            nodes[i + width - 1]->AddEdge(nodes[i]);
        }
    }
}