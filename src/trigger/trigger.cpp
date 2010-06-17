#include "trigger.h"
#include "util/token.h"
#include "level/scene.h"
#include "environment/atmosphere.h"
#include <vector>

using namespace std;

class TriggerEnvironment: public Trigger {
public:
    TriggerEnvironment(Token * token){
        message = token->findToken("trigger/message");
        if (message == NULL){
            throw LoadException(__FILE__, __LINE__, "Expected trigger/message token");
        }
        message = message->copy();
    }

    virtual void execute(Scene * scene){
        const vector<Atmosphere*> & atmospheres = scene->getAtmospheres();
        for (vector<Atmosphere *>::const_iterator it = atmospheres.begin(); it != atmospheres.end(); it++){
            Atmosphere * atmosphere = *it;
            atmosphere->interpret(message);
        }
    }

    virtual bool shouldExecute() const {
        return true;
    }

    virtual ~TriggerEnvironment(){
        delete message;
    }
protected:
    Token * message;
};

Trigger * Trigger::parse(Token * token) throw (TokenException) {
    Token * token_type = token->findToken("trigger/type");
    if (token_type == NULL){
        throw TokenException(__FILE__, __LINE__, "Expected to find trigger/type");
    }
    string type;
    *token_type >> type;
    if (type == "environment"){
        return new TriggerEnvironment(token);
    }
    throw LoadException(__FILE__, __LINE__, "Unknown trigger type " + type);
}
    
Trigger::Trigger(){
}

Trigger::~Trigger(){
}
