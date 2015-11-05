var PlayerHud = function(){};
PlayerHud.prototype = {
    playingObj: null,
    playingUid: null,
    isPlaying: 0,
    isRecording: 0,
    volume: 100,

    updateBufforStatus: function(status) {
        this.playingObj.find('.status').text(status);
    },

    clearBufforStatus: function() {
        this.playingObj.find('.status').hide(1000);
        this.playingObj.removeClass('play').addClass('stop');
        this.isPlaying = 1;
    },

    play: function(clickedObj) {
        if( this.isPlaying)
        {
            this.stop();
        }
        this.playingObj = clickedObj;
        this.playingUid = this.playingObj.attr('data-uid');
        Player.play(this.playingUid);
    },

    stop: function() {
        this.playingObj = null;
        this.isPlaying = 0;
        Player.stop();
    },

    getClicked: function() {
        return this.playingObj;
    },

    setVolume: function(volume) {
        this.volume = volume;
        Player.setVolume(volume);
    },

    getValue: function() {
        try
        {
            this.value = Player.getValue();
        }
        catch(err)
        {
            //QT object not loaded
            console.log("Object or method not exists: " + err);
            this.value = 100;
        }
        return this.value;
    },

    record: function(){
        if( this.isPlaying)
        {
            Player.record();
        }
    }
};
